// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESniperScopeWidget.generated.h"

UCLASS()
class EPORTFOLIO_API UESniperScopeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayZoomIn(bool bIsAimming);

public :
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Animation_ZoomIn;
};
