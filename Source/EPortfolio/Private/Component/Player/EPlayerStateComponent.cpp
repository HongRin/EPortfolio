// Copyright EPortfolio


#include "Component/Player/EPlayerStateComponent.h"
#include "Character/Player/EPlayer.h"

void UEPlayerStateComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<AEPlayer>(GetOwner());

	checkf(OwnerPlayer, TEXT("Owner player is null ! "))

	OwnerPlayer->LandedDelegate.AddDynamic(this, &ThisClass::DisabledJumpping);
}

void UEPlayerStateComponent::SetJump()
{
	if (Jumpping)
	{
		return;
	}

	OwnerPlayer->Super::Jump();
	Jumpping = true;
}

void UEPlayerStateComponent::DisabledJumpping(const FHitResult& Hit)
{
	Jumpping = false;
}
