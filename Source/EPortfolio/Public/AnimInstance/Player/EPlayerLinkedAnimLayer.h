// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/EBaseAnimInstance.h"
#include "EPlayerLinkedAnimLayer.generated.h"

UCLASS()
class EPORTFOLIO_API UEPlayerLinkedAnimLayer : public UEBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	class UEPlayerAnimInstance* GetPlayerAnimInstance() const;
};
