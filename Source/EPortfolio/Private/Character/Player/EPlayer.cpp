// Copyright EPortfolio


#include "Character/Player/EPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/Player/EPlayerStateComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "HUD/EOverheadWidget.h"

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
}

void AEPlayer::BeginPlay()
{
	Super::BeginPlay();
	Cast<UEOverheadWidget>(OverheadWidget->GetUserWidgetObject())->ShowPlayerName(this);

	check(IMCPlayer);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		checkf(Subsystem, TEXT("SubSystem is Null ! "));
		{
			Subsystem->AddMappingContext(IMCPlayer, 0);
		}
	}
}

void AEPlayer::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}

void AEPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Started  , this, &ThisClass::JumpAction);
		EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &ThisClass::MoveAction);
		EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &ThisClass::LookAction);
	}
}

void AEPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
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



