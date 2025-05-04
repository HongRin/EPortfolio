// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FinishedReloadNotify.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API UFinishedReloadNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public :
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
