// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/ECharacterAnimInstance.h"
#include "EPlayerAnimInstance.generated.h"

UCLASS()
class EPORTFOLIO_API UEPlayerAnimInstance : public UECharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected :
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsJumpping;
};
