// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SetCanMoveAnimNotifyState.generated.h"

UCLASS()
class EPORTFOLIO_API USetCanMoveAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public :
	FString GetNotifyName() const;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
