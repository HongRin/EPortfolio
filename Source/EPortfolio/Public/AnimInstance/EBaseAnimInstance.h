// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EBaseAnimInstance.generated.h"

UCLASS()
class EPORTFOLIO_API UEBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public :
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected :
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<class AEBaseCharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsAccelerating;
};
