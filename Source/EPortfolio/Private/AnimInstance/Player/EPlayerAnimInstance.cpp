// Copyright EPortfolio


#include "AnimInstance/Player/EPlayerAnimInstance.h"
#include "AnimInstance/Player/EPlayerLinkedAnimLayer.h"
#include "Character/Player/EPlayer.h"

void UEPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UEPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerCharacter)
	{
		bIsJumpping = OwnerCharacter->GetVelocity().Z > 100;
		bIsCrouched = OwnerCharacter->bIsCrouched;
		if (AEPlayer* Player = Cast<AEPlayer>(OwnerCharacter))
		{
			bIsAiming = Player->IsAiming();
		}
	}

	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}
