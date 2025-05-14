// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "HUD/EBaseMenuWidget.h"
#include "ELobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API UELobbyWidget : public UEBaseMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

public:
	virtual void MenuSetup() override;
	virtual void MenuTearDown() override;

private :
	UFUNCTION()
	void OnPlayerCountChanged(int32 NewCount);
	
	UFUNCTION()
	void StartButtonClicked();

public :
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_PlayerCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Start;
};
