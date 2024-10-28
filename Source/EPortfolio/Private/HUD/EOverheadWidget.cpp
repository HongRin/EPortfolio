// Copyright EPortfolio


#include "HUD/EOverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

void UEOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}

void UEOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UEOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	if (const APlayerState* PlayerState = InPawn->GetPlayerState())
	{
		SetDisplayText(PlayerState->GetPlayerName());
		return;
	}
	
	SetDisplayText("NULL");
}
