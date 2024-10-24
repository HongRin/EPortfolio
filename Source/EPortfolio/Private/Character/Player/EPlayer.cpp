// Copyright EPortfolio


#include "Character/Player/EPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/Player/EPlayerStateComponent.h"

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
		PlayerState = CreateDefaultSubobject<UEPlayerStateComponent>(TEXT("PlayerState"));
	}
}

void AEPlayer::Move(const FVector2D& InMovementVector, const FRotator& InMovementRotation)
{
	if (!PlayerState->IsCanMove())
	{
		return;
	}

	if (InMovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = InMovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, InMovementVector.Y);
	}

	if (InMovementVector.X != 0.f)
	{
		const FVector RightDirection = InMovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, InMovementVector.X);
	}
}

void AEPlayer::Look(const double& InX, const double& InY)
{
	if (InX != 0.f)
	{
		AddControllerYawInput(InX);
	}

	if (InY != 0.f)
	{
		AddControllerPitchInput(InY);
	}
}

void AEPlayer::Jump()
{
	PlayerState->SetJump();
}

void AEPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}



