// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/EBaseAnimInstance.h"
#include "ECharacterAnimInstance.generated.h"

UCLASS()
class EPORTFOLIO_API UECharacterAnimInstance : public UEBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected :
	FVector Velocity;

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
 