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
		UEPlayerStateComponent* StateComponent = Cast<AEPlayer>(OwnerCharacter)->GetStateComponent();
		bIsJumpping = StateComponent->IsJumpping();
	}

	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}
