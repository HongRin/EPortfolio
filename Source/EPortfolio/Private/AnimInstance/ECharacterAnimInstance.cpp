// Copyright EPortfolio


#include "AnimInstance/ECharacterAnimInstance.h"
#include "Character/EBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


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

	Velocity		= OwnerCharacter->GetVelocity();
	Speed           = Velocity.Size2D();
	bIsInAir        = OwnerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
}
