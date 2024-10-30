// Copyright EPortfolio


#include "Character/Player/EPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Components/WidgetComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "Net/UnrealNetwork.h"

#include "HUD/EOverheadWidget.h"
#include "Weapon/EWeapon.h"
#include "Component/Player/EPlayerStateComponent.h"
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
		StateComponent = CreateDefaultSubobject<UEPlayerStateComponent>(TEXT("StateComponent"));
	}

	{
		OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
		OverheadWidget->SetupAttachment(RootComponent);
	}

	{
		CombatComponent = CreateDefaultSubobject<UECombatComponent>(TEXT("CombatComponent"));
		CombatComponent->SetIsReplicated(true);
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

		EnhancedInputComponent->BindAction(IAMove , ETriggerEvent::Triggered, this, &ThisClass::MoveAction );
		EnhancedInputComponent->BindAction(IALook , ETriggerEvent::Triggered, this, &ThisClass::LookAction );
		EnhancedInputComponent->BindAction(IAJump , ETriggerEvent::Started  , this, &ThisClass::JumpAction );
		EnhancedInputComponent->BindAction(IAEquip, ETriggerEvent::Started  , this, &ThisClass::EquipAction);
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
	if (CombatComponent)
	{
		if (HasAuthority())
		{
			CombatComponent->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquip();
		}
	}
}

void AEPlayer::ServerEquip_Implementation()
{
	if (CombatComponent)
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
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
		StateComponent->SetJump();
	}
}

void AEPlayer::EquipAction(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		if (CombatComponent && HasAuthority())
		{
			CombatComponent->EquipWeapon(OverlappingWeapon);
		}
	}
}



