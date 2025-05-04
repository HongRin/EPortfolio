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
	FString RemoteRoleString;
	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		RemoteRoleString = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		RemoteRoleString = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		RemoteRoleString = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		RemoteRoleString = FString("None");
		break;
	}

	ENetRole LocalRole = InPawn->GetLocalRole();
	FString LocalRoleString;
	switch (LocalRole)
	{
	case ENetRole::ROLE_Authority:
		LocalRoleString = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		LocalRoleString = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		LocalRoleString = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		LocalRoleString = FString("None");
		break;
	}

	//FString RoleString = FString::Printf(TEXT("%s : %s"), *RemoteRoleString, *LocalRoleString);
	//DisplayText->SetText(FText::FromString(RoleString));	

}
