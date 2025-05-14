// Copyright EPortfolio


#include "Game/ELobbyGameState.h"
#include "Net/UnrealNetwork.h"

void AELobbyGameState::UpdatePlayerCount(int32 Count)
{
	CurrentPlayers += Count;
	OnPlayerCountChanged.Broadcast(CurrentPlayers);
}

void AELobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AELobbyGameState, CurrentPlayers);
}

void AELobbyGameState::OnRep_PlayerCount()
{
	OnPlayerCountChanged.Broadcast(CurrentPlayers);
}
