// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EAnnouncementWidget.generated.h"

UCLASS()
class EPORTFOLIO_API UEAnnouncementWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_WarmupTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Announcement;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Info;
};
