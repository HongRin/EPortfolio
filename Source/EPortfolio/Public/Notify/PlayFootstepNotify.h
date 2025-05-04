// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "PlayFootstepNotify.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API UPlayFootstepNotify : public UAnimNotify_PlaySound
{
	GENERATED_BODY()
	
public :
	UPlayFootstepNotify();
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
