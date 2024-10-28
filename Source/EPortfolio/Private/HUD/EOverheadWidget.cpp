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
	checkf(InPawn, TEXT("InPawn is not valid"));
	checkf(InPawn->GetPlayerState(), TEXT("GetPlayerState is not valid"));
	checkf(*InPawn->GetPlayerState()->GetPlayerName(), TEXT("GetPlayerName is not valid"));

	FString PlayerName = InPawn->GetPlayerState()->GetPlayerName();
	
	SetDisplayText(PlayerName);
}
