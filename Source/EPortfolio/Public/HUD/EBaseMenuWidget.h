// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EBaseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API UEBaseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void MenuSetup();

	virtual void MenuTearDown();

protected :
	UPROPERTY()
	class APlayerController* PlayerController;
	
};
