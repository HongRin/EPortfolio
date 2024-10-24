// Copyright EPortfolio


#include "AnimInstance/Player/EPlayerAnimInstance.h"
#include "Character/Player/EPlayer.h"
#include "Component/Player/EPlayerStateComponent.h"

void UEPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UEPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerCharacter)
	{
		UEPlayerStateComponent* PlayerState = Cast<AEPlayer>(OwnerCharacter)->GetPlayerState();
		bIsJumpping = PlayerState->IsJumpping();
	}

	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}
