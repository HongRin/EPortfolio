// Copyright EPortfolio


#include "AnimInstance/EBaseAnimInstance.h"
#include "Character/EBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEBaseAnimInstance::NativeInitializeAnimation()
{
	OwnerCharacter = Cast<AEBaseCharacter>(TryGetPawnOwner());
}

void UEBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerCharacter == nullptr)
	{ 
		return; 
	}

	Speed = OwnerCharacter->GetVelocity().Size2D();
	
	bIsInAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

}
