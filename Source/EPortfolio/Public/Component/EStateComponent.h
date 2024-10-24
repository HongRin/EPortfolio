// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EStateComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EPORTFOLIO_API UEStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEStateComponent();
};
