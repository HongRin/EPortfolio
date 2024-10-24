// Copyright EPortfolio


#include "Noitfy/SetCanMoveAnimNotifyState.h"
#include "Component/Player/EPlayerStateComponent.h"

FString USetCanMoveAnimNotifyState::GetNotifyName() const
{
	return TEXT("SetCanMove");
}

void USetCanMoveAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	UEPlayerStateComponent* PlayerState = MeshComp->GetOwner()->FindComponentByClass<UEPlayerStateComponent>();
	if (PlayerState)
	{
		PlayerState->DisableCanMove();
	}
}

void USetCanMoveAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	UEPlayerStateComponent* PlayerState = MeshComp->GetOwner()->FindComponentByClass<UEPlayerStateComponent>();
	if (PlayerState)
	{
		PlayerState->EnableCanMove();
	}
}
