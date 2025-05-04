// Copyright EPortfolio


#include "Notify/PlayFootstepNotify.h"
#include "Character/Player/EPlayer.h"

UPlayFootstepNotify::UPlayFootstepNotify()
{
	bFollow                   = false;
}

FString UPlayFootstepNotify::GetNotifyName_Implementation() const
{
	return FString("FootStep");
}

void UPlayFootstepNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AEPlayer* Player = Cast<AEPlayer>(MeshComp->GetOwner()))
	{
		VolumeMultiplier = Player->GetFootStepVolume();
	}
	Super::Notify(MeshComp, Animation, EventReference);
}
