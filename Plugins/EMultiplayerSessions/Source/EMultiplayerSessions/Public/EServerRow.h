// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EServerRow.generated.h"

UCLASS()
class EMULTIPLAYERSESSIONS_API UEServerRow : public UUserWidget
{
	GENERATED_BODY()

public :
	void Setup(class UEMenu* Parent, uint32 Index);

	void SetRowColor();

private :
	UFUNCTION()
	void RowButtonClicked();

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_ServerName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_HostUser;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_ConnectionFraction;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Row;

	UPROPERTY()
	TObjectPtr<class UEMenu> Parent;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	uint32 Index;
};
