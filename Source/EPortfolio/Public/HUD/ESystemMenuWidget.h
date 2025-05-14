// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "HUD/EBaseMenuWidget.h"
#include "ESystemMenuWidget.generated.h"

UCLASS()
class EPORTFOLIO_API UESystemMenuWidget : public UEBaseMenuWidget
{
	GENERATED_BODY()

public:
	virtual void MenuSetup() override;
	virtual void MenuTearDown() override;

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

private :
	UFUNCTION()
	void ReturnButtonClicked();

	UFUNCTION()
	void ReturnButtonHovered();

	UFUNCTION()
	void ReturnButtonUnhovered();

	UFUNCTION()
	void LeaveGameButtonClicked();

	UFUNCTION()
	void LeaveGameButtonHovered();

	UFUNCTION()
	void LeaveGameButtonUnhovered();

	UFUNCTION()
	void ExitToDesktopButtonClicked();

	UFUNCTION()
	void ExitToDesktopButtonHovered();

	UFUNCTION()
	void ExitToDesktopButtonUnhovered();

	
private :
	void SetButtonTextColor(class UTextBlock* TextBlock, const FLinearColor & Color);


public :
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Return;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_LeaveGame;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Exittodesktop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Return;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_LeaveGame;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_ExitToDesktop;

	UPROPERTY()
	class UEMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY(EditAnywhere)
	FLinearColor HoveredTextColor;

	UPROPERTY(EditAnywhere)
	FLinearColor ClickedTextColor;

	bool bShowSystemMenu = false;
};
