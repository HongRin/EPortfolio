// Copyright EPortfolio


#include "Game/ELobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "Game/ELobbyGameState.h"
#include "Controller/EPlayerController.h"
#include "EMultiplayerSessionsSubsystem.h"

void AELobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AELobbyGameState* LobbyGameState = GetGameState<AELobbyGameState>())
	{
		LobbyGameState->UpdatePlayerCount(1);

		if (UGameInstance* GameInstance = GetGameInstance())
		{
			UEMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UEMultiplayerSessionsSubsystem>();

			if (!MultiplayerSessionsSubsystem) return;

			MultiplayerSessionsSubsystem->SetPlayerCount(LobbyGameState->GetCurrentPlayers());
		}
	}
}

void AELobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (AELobbyGameState* LobbyGameState = GetGameState<AELobbyGameState>())
	{
		LobbyGameState->UpdatePlayerCount(-1);
	}
}

void AELobbyGameMode::StartGame()
{
	if (UWorld* World = GetWorld())
	{
		if (GameState.Get()->PlayerArray.Num() >= 2)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Levels/Shooter?listen"));
		}
	}
}
