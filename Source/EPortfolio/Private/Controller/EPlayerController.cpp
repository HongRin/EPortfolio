// Copyright EPortfolio


#include "Controller/EPlayerController.h"
#include "Character/Player/EPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

AEPlayerController::AEPlayerController()
{
	bReplicates = true;

}

void AEPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Player = Cast<AEPlayer>(InPawn);
}

void AEPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(IMCPlayer);

	UEnhancedInputLocalPlayerSubsystem* subSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(subSystem);

	subSystem->AddMappingContext(IMCPlayer, 0);
}

void AEPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &ThisClass::MoveAction);

	EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &ThisClass::LookAction);

	EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Started, this, &ThisClass::JumpAction);
}

void AEPlayerController::MoveAction(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, GetControlRotation().Yaw, 0.f);

	Player->Move(MovementVector, MovementRotation);
}

void AEPlayerController::LookAction(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	Player->Look(LookAxisVector.X, LookAxisVector.Y);
}

void AEPlayerController::JumpAction(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		Player->Jump();
	}
}
