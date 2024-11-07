// Copyright EPortfolio


#include "Character/Player/EPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/WidgetComponent.h"
#include "AnimInstance/Player/EPlayerLinkedAnimLayer.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "Net/UnrealNetwork.h"

#include "HUD/EOverheadWidget.h"
#include "Weapon/EWeapon.h"
#include "Component/ECombatComponent.h"


AEPlayer::AEPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	{
		CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
		CameraBoom->SetupAttachment(GetMesh());
		CameraBoom->TargetArmLength = 600.f;
		CameraBoom->bUsePawnControlRotation = true;
	}

	{
		FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false;
	}

	{
		OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
		OverheadWidget->SetupAttachment(RootComponent);
	}

	{
		CombatComponent = CreateDefaultSubobject<UECombatComponent>(TEXT("CombatComponent"));
		CombatComponent->SetIsReplicated(true);
	}

	bUseControllerRotationYaw   = true;
	bUseControllerRotationRoll  = false;
	bUseControllerRotationPitch = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bWantsToCrouch = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 400.f;

	GetMesh()->SetIsReplicated(true);
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

	Cast<UEOverheadWidget>(OverheadWidget->GetUserWidgetObject())->ShowPlayerName(this);
}

void AEPlayer::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

}

void AEPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}


void AEPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(IAMove    , ETriggerEvent::Triggered, this, &ThisClass::MoveAction );
		EnhancedInputComponent->BindAction(IALook    , ETriggerEvent::Triggered, this, &ThisClass::LookAction );
		EnhancedInputComponent->BindAction(IAJump    , ETriggerEvent::Started  , this, &ThisClass::JumpAction );
		EnhancedInputComponent->BindAction(IAEquip   , ETriggerEvent::Started  , this, &ThisClass::EquipAction);
		EnhancedInputComponent->BindAction(IACrouch  , ETriggerEvent::Started  , this, &ThisClass::CrouchAction);
		EnhancedInputComponent->BindAction(IARun     , ETriggerEvent::Triggered, this, &ThisClass::RunAction);
		EnhancedInputComponent->BindAction(IASlowWalk, ETriggerEvent::Triggered, this, &ThisClass::SlowWalkAction);
		EnhancedInputComponent->BindAction(IAAiming  , ETriggerEvent::Triggered, this, &ThisClass::AimingAction);
	}
}

void AEPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AEPlayer, OverlappingWeapon, COND_OwnerOnly);
}

void AEPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->Player = this;
	}
}

void AEPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
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

void AEPlayer::ServerEquip_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
}

void AEPlayer::ServerSetMaxSpeed_Implementation(float MaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
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

void AEPlayer::SetMaxSpeed(float MaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	ServerSetMaxSpeed(MaxSpeed);
}

void AEPlayer::MoveAction(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AEPlayer::LookAction(const FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AEPlayer::JumpAction(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
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
			CombatComponent->EquipWeapon(OverlappingWeapon);
			ServerEquip();
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
	if (InputActionValue.Get<bool>())
	{
		SetMaxSpeed(600.f);
	}
	else
	{
		SetMaxSpeed(400.f);
	}
}

void AEPlayer::SlowWalkAction(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		SetMaxSpeed(300.f);
	}
	else
	{
		SetMaxSpeed(400.f);
	}
}

void AEPlayer::AimingAction(const FInputActionValue& InputActionValue)
{
	CombatComponent->SetAiming(InputActionValue.Get<bool>());
}

bool AEPlayer::IsAiming()
{
	return (CombatComponent && CombatComponent->bAiming);
}

