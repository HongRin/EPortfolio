// Copyright EPortfolio


#include "HUD/EOverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UEOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}

void UEOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	//GetWorld()->GetTimerManager().SetTimer(CheckPlayerStateTimerHandle,
	//	[InPawn, this]()
	//	{
	//		if (const APlayerState* PlayerState = InPawn->GetPlayerState())
	//		{
	//			DisplayText->SetText(FText::FromString(PlayerState->GetPlayerName()));
	//			GetWorld()->GetTimerManager().ClearTimer(CheckPlayerStateTimerHandle);
	//		}
	//		else
	//		{
	//			DisplayText->SetText(FText::FromString("NULL"));
	//		}
	//	},
	//	0.1f, true
	//);
}
