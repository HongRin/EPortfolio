// Copyright EPortfolio


#include "HUD/ELobbyWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/ELobbyGameState.h"
#include "Game/ELobbyGameMode.h"


bool UELobbyWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Button_Start)
	{
		Button_Start->OnClicked.AddDynamic(this, &UELobbyWidget::StartButtonClicked);
	}

	return true;
}

void UELobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetWorld())
	{
		if (AELobbyGameState* LobbyGameState = GetWorld()->GetGameState<AELobbyGameState>())
		{
			LobbyGameState->OnPlayerCountChanged.AddDynamic(this, &UELobbyWidget::OnPlayerCountChanged);
			OnPlayerCountChanged(LobbyGameState->GetCurrentPlayers());
		}
	}

	
}

void UELobbyWidget::MenuSetup()
{
	Super::MenuSetup();

	if (UWorld* World = GetWorld())
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			if (PlayerController->HasAuthority())
			{
				Button_Start->SetIsEnabled(false);
			}
			else
			{
				Button_Start->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UELobbyWidget::MenuTearDown()
{
	Super::MenuTearDown();
}

void UELobbyWidget::OnPlayerCountChanged(int32 NewCount)
{
	FString Text;

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString(TEXT("OnPlayerCountChanged")));

	if (UWorld* World = GetWorld())
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			if (PlayerController->HasAuthority())
			{
				if (NewCount >= 2)
				{
					Button_Start->SetIsEnabled(true);
				}
				else
				{
					Button_Start->SetIsEnabled(false);
				}
			}
		}
		
		Text = FString::Printf(TEXT("Players : %d / %d"), NewCount, 4);
		TextBlock_PlayerCount->SetText(FText::FromString(FString(Text)));
	}

}

void UELobbyWidget::StartButtonClicked()
{
	if (UWorld* World = GetWorld())
	{
		if (AELobbyGameMode* GameMode = World->GetAuthGameMode<AELobbyGameMode>())
		{
			GameMode->StartGame();
		}
	}
}
