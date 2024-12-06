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
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	}
	FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
	DisplayText->SetText(FText::FromString(RemoteRoleString));

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
