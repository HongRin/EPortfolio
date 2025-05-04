// Copyright EPortfolio


#include "Game/ELobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void AELobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Levels/Shooter?listen"));
		}
	}
}
