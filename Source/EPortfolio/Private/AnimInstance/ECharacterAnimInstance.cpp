// Copyright EPortfolio


#include "AnimInstance/ECharacterAnimInstance.h"
#include "Character/EBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UECharacterAnimInstance::NativeInitializeAnimation()
{
	OwnerCharacter = Cast<AEBaseCharacter>(TryGetPawnOwner());
}

void UECharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	Speed = OwnerCharacter->GetVelocity().Size2D();

	bIsInAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
}
