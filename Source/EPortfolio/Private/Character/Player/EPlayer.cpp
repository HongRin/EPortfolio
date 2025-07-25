// Copyright EPortfolio

#include "Character/Player/EPlayer.h"
#include "EPortfolio/EPortfolio.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/WidgetComponent.h"
#include "AnimInstance/Player/EPlayerLinkedAnimLayer.h"
#include "AnimInstance/Player/EPlayerAnimInstance.h"


#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "Net/UnrealNetwork.h"

#include "HUD/EOverheadWidget.h"
#include "Weapon/EWeapon.h"
#include "Component/ECombatComponent.h"
#include "Component/EBuffComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Controller/EPlayerController.h"
#include "Game/EGameMode.h"
#include "Game/EPlayerState.h"

#include "ELogHelpers.h"


AEPlayer::AEPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	{
		CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
		CameraBoom->SetupAttachment(GetRootComponent());
		CameraBoom->TargetArmLength = 600.f;
		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->bEnableCameraLag = true;
		CameraBoom->bEnableCameraRotationLag= true;
	}

	{
		FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false;
	}

	{
		CombatComponent = CreateDefaultSubobject<UECombatComponent>(TEXT("CombatComponent"));
		CombatComponent->SetIsReplicated(true);
	}

	{
		BuffComponent = CreateDefaultSubobject<UEBuffComponent>(TEXT("BuffComponent"));
		BuffComponent->SetIsReplicated(true);
	}

	{
		DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));
	}

	{
		bUseControllerRotationYaw = true;
		bUseControllerRotationRoll = false;
		bUseControllerRotationPitch = false;
	}

	{
		RunSpeed        = 600.f;
		WalkSpeed       = 400.f;
		CrouchSpeed     = 330.f;
		SlowWalkSpeed   = 250.f;
	}

	{
		GetCharacterMovement()->bWantsToCrouch = true;
		GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
		GetCharacterMovement()->CrouchedHalfHeight = 60.f;
	}

	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECC_Shot, ECollisionResponse::ECR_Block);
	}
}

void AEPlayer::BeginPlay()
{
	Super::BeginPlay();

	check(IMCPlayer);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		checkf(Subsystem, TEXT("SubSystem is Null ! "));
		{
			Subsystem->AddMappingContext(IMCPlayer, 0);
		}
	}

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &AEPlayer::ReceiveDamage);
	}

	UpdateHealth();
	UpdateShield();

}

void AEPlayer::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	PollInit();
}

void AEPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	check(IMCPlayer);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMCPlayer, 0);
		}
	}
}


void AEPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(IAMove      , ETriggerEvent::Triggered, this, &ThisClass::MoveAction      );
		EnhancedInputComponent->BindAction(IALook      , ETriggerEvent::Triggered, this, &ThisClass::LookAction      );
		EnhancedInputComponent->BindAction(IAJump      , ETriggerEvent::Started  , this, &ThisClass::JumpAction      );
		EnhancedInputComponent->BindAction(IAEquip     , ETriggerEvent::Started  , this, &ThisClass::EquipAction     );
		EnhancedInputComponent->BindAction(IACrouch    , ETriggerEvent::Started  , this, &ThisClass::CrouchAction    );
		EnhancedInputComponent->BindAction(IARun       , ETriggerEvent::Started  , this, &ThisClass::RunAction       );
		EnhancedInputComponent->BindAction(IARun       , ETriggerEvent::Completed, this, &ThisClass::RunAction	     );
		EnhancedInputComponent->BindAction(IASlowWalk  , ETriggerEvent::Started  , this, &ThisClass::SlowWalkAction  );
		EnhancedInputComponent->BindAction(IASlowWalk  , ETriggerEvent::Completed, this, &ThisClass::SlowWalkAction  );
		EnhancedInputComponent->BindAction(IAAiming    , ETriggerEvent::Started  , this, &ThisClass::AimingAction    );
		EnhancedInputComponent->BindAction(IAAiming    , ETriggerEvent::Completed, this, &ThisClass::AimingAction    );
		EnhancedInputComponent->BindAction(IAFire      , ETriggerEvent::Started  , this, &ThisClass::FiringAction    );
		EnhancedInputComponent->BindAction(IAFire	   , ETriggerEvent::Completed, this, &ThisClass::FiringAction    );
		EnhancedInputComponent->BindAction(IAReload    , ETriggerEvent::Started  , this, &ThisClass::ReloadAction    );
		EnhancedInputComponent->BindAction(IASystemMenu, ETriggerEvent::Started  , this, &ThisClass::SystemMenuAction);
	}
}

void AEPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AEPlayer, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(AEPlayer, Health);
	DOREPLIFETIME(AEPlayer, Shield);
}

void AEPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->Player = this;
	}
	if (BuffComponent)
	{
		BuffComponent->Player = this;
	}
}

void AEPlayer::OnRep_OverlappingWeapon(AEWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void AEPlayer::OnRep_Health()
{
	UpdateHealth();
}

void AEPlayer::OnRep_Shield()
{
	UpdateShield();
}



void AEPlayer::ServerEquip_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
}

void AEPlayer::SetMaxSpeed(float MaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	ServerSetMaxSpeed(MaxSpeed);
}

void AEPlayer::MulticastSetMaxSpeed_Implementation(float MaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}

void AEPlayer::ServerSetMaxSpeed_Implementation(float MaxSpeed)
{
	MulticastSetMaxSpeed(MaxSpeed);
}

void AEPlayer::MulticastHit_Implementation(FVector2D Direction)
{
	if (CombatComponent)
	{
		CombatComponent->PlayHitReactMontage(Direction);
	}
}

void AEPlayer::MulticastElim_Implementation()
{

	bElimmed = true;
	PlayAnimMontage(ElimMontage);

	if (CombatComponent)
	{
		CombatComponent->bFiring = false;
	}


	if (!DissolveMaterialInstances.IsEmpty())
	{
		for (int i = 0; i < DissolveMaterialInstances.Num(); i++)
		{
			DynamicDissolveMaterialInstances.Add(UMaterialInstanceDynamic::Create(DissolveMaterialInstances[i].Get(), this));
			GetMesh()->SetMaterial(i, DynamicDissolveMaterialInstances[i]);
			DynamicDissolveMaterialInstances[i]->SetScalarParameterValue(TEXT("Dissolve"), -0.5f);
			DynamicDissolveMaterialInstances[i]->SetScalarParameterValue(TEXT("Intensity"), 35.f);
		}

		StartDissolve();
	}

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	if (EPlayerController)
	{
		DisableInput(EPlayerController);
		EPlayerController->UpdateAmmoHUD(0);

		if (CombatComponent && CombatComponent->EquippedWeapon)
		{
			if (CombatComponent->EquippedWeapon->GetWeaponType() == EWeaponType::WT_Sniper)
			{
				EPlayerController->UpdateSniperScopeHUD(false);
			}
		}
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEPlayer::SetOverlappingWeapon(AEWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}



void AEPlayer::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	if (bElimmed) return;

	float DamageToHealth = Damage;

	if (Shield > 0.f)
	{
		if (Shield >= Damage)
		{
			Shield = FMath::Clamp(Shield - Damage, 0.f, MaxShield);
			DamageToHealth = 0.f;
		}
		else
		{
			DamageToHealth = FMath::Clamp(DamageToHealth - Shield, 0.f, Damage);
			Shield = 0.f;
		}
	}

	if (Shield >= 0)
	{
		UpdateShield();
	}

	if (DamageToHealth > 0)
	{
		Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);
		UpdateHealth();
	}

	

	if (Health == 0.f)
	{
		AEGameMode* GameMode = GetWorld()->GetAuthGameMode<AEGameMode>();
		if (GameMode)
		{
			EPlayerController = EPlayerController == nullptr ? Cast<AEPlayerController>(Controller) : EPlayerController.Get();
			AEPlayerController* AttackerController = Cast<AEPlayerController>(InstigatorController);
			GameMode->PlayerEliminated(this, EPlayerController, AttackerController);
		}
	}
}

void AEPlayer::UpdateHealth()
{
	EPlayerController = EPlayerController == nullptr ? Cast<AEPlayerController>(Controller) : EPlayerController.Get();
	if (EPlayerController)
	{
		EPlayerController->UpdateHealthHUD(Health, MaxHealth);
	}
}

void AEPlayer::UpdateShield()
{
	EPlayerController = EPlayerController == nullptr ? Cast<AEPlayerController>(Controller) : EPlayerController.Get();
	if (EPlayerController)
	{
		EPlayerController->UpdateShieldHUD(Shield, MaxShield);
	}
}

void AEPlayer::Elim()
{
	if (CombatComponent && CombatComponent->EquippedWeapon)
	{
		CombatComponent->EquippedWeapon->Dropped();
	}
	CombatComponent->MouseSensitivity = 1.f;
	MulticastElim();
	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&AEPlayer::ElimTimerFinished,
		ElimDelay
	);
}

void AEPlayer::ElimTimerFinished()
{
	AEGameMode* GameMode = GetWorld()->GetAuthGameMode<AEGameMode>();
	if (GameMode)
	{
		GameMode->RequestRespawn(this, Controller);
	}
}

void AEPlayer::UpdateDissolveMaterial(float DissolveValue)
{
	if (!DynamicDissolveMaterialInstances.IsEmpty())
	{
		DynamicDissolveMaterialInstances[0]->SetScalarParameterValue(TEXT("Dissolve"), DissolveValue);
		DynamicDissolveMaterialInstances[1]->SetScalarParameterValue(TEXT("Dissolve"), DissolveValue);
	}
}

void AEPlayer::StartDissolve()
{
	DissolveTrack.BindDynamic(this, &AEPlayer::UpdateDissolveMaterial);
	if (DissolveCurve && DissolveTimeline)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);
		DissolveTimeline->Play();
	}
}

void AEPlayer::PollInit()
{
	if (EPlayerState == nullptr)
	{
		EPlayerState = GetPlayerState<AEPlayerState>();
		if (EPlayerState)
		{
			EPlayerState->AddToKillScore(0.f);
			EPlayerState->AddToDeathScore(0);
		}
	}
}


void AEPlayer::MoveAction(const FInputActionValue& InputActionValue)
{
	const FRotator Rotation = GetControlRotation();

	FVector RightDirection = UKismetMathLibrary::GetRightVector(FRotator(0.f, Rotation.Yaw, Rotation.Roll));

	AddMovementInput(RightDirection, InputActionValue.Get<FVector2D>().X);

	FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0.f, Rotation.Yaw, 0.f));

	AddMovementInput(ForwardDirection, InputActionValue.Get<FVector2D>().Y);
}

void AEPlayer::LookAction(const FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr && CombatComponent != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * CombatComponent->MouseSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * CombatComponent->MouseSensitivity);
	}
}

void AEPlayer::JumpAction(const FInputActionValue& InputActionValue)
{
	if (!bIsCrouched)
	{
		Super::Jump();
	}
}

void AEPlayer::EquipAction(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		if (CombatComponent)
		{
			if (HasAuthority())
			{
				CombatComponent->EquipWeapon(OverlappingWeapon);
				CombatComponent->SetItemAnimLayer();
			}
			else
			{
				ServerEquip();
				CombatComponent->ServerSetItemAnimLayer();
			}
		}
	}
}

void AEPlayer::CrouchAction(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		if (bIsCrouched)
		{
			UnCrouch();
		}
		else
		{
			Crouch();
		}
	}
}

void AEPlayer::RunAction(const FInputActionValue& InputActionValue)
{
	if (IsAiming() || IsFiring()) return;

	if (InputActionValue.Get<bool>())
	{
		SetMaxSpeed(RunSpeed);
	}
	else
	{
		SetMaxSpeed(WalkSpeed);
	}
}

void AEPlayer::SlowWalkAction(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		SetMaxSpeed(SlowWalkSpeed);
	}
	else
	{
		SetMaxSpeed(WalkSpeed);
	}
}

void AEPlayer::AimingAction(const FInputActionValue& InputActionValue)
{
	CombatComponent->SetAiming(InputActionValue.Get<bool>());
	
	if (InputActionValue.Get<bool>())
	{
		SetMaxSpeed(WalkSpeed);
	}
}

void AEPlayer::FiringAction(const FInputActionValue& InputActionValue)
{
	if (CombatComponent == nullptr) return;
	
	CombatComponent->Firing(InputActionValue.Get<bool>());

	SetMaxSpeed(WalkSpeed);

}

void AEPlayer::ReloadAction(const FInputActionValue& InputActionValue)
{
	if (CombatComponent == nullptr) return;

	CombatComponent->Reload();
}

void AEPlayer::SystemMenuAction(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		if (IsLocallyControlled())
		{
			EPlayerController = EPlayerController == nullptr ? Cast<AEPlayerController>(Controller) : EPlayerController.Get();

			EPlayerController->ToggleSystemMenu();
		}
	}
}

bool AEPlayer::IsAiming()
{
	return (CombatComponent && CombatComponent->bAiming && CombatComponent->EquippedWeapon);
}

bool AEPlayer::IsFiring()
{
	return (CombatComponent && CombatComponent->bFiring && CombatComponent->EquippedWeapon);
}

TSubclassOf<class UEPlayerLinkedAnimLayer> AEPlayer::GetAnimLayer()
{
	return CombatComponent->ItemAnimLayer;
}

FVector AEPlayer::GetHitTarget() const
{
	if (CombatComponent == nullptr) return FVector();
	return CombatComponent->HitTarget;
}

float AEPlayer::GetFootStepVolume() const
{
	return 	GetVelocity().Size2D() / RunSpeed;
}

ECombatState AEPlayer::GetCombatState() const
{
	if (CombatComponent == nullptr) return ECombatState::ECS_MAX;
	return CombatComponent->CombatState;
}

AEWeapon* AEPlayer::GetEquippedWeapon()
{
	if (CombatComponent == nullptr) return nullptr;
	return CombatComponent->EquippedWeapon;
}

