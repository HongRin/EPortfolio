// Copyright EPortfolio


#include "HUD/EOverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

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
	// FString PlayerName = InPawn->GetPlayerState()->GetPlayerName();
	
	//SetDisplayText(PlayerName);
}
