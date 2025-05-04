// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EOverheadWidget.generated.h"

UCLASS()
class EPORTFOLIO_API UEOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	virtual void NativeDestruct() override;

public :
	void ShowPlayerName(APawn* InPawn);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DisplayText;
};
