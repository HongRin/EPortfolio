// Copyright EPortfolio


#include "Game/EGameMode.h"
#include "Character/Player/EPlayer.h"
#include "Controller/EPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/EPlayerState.h"
#include "Game/EGameState.h"
#include "EMultiplayerSessionsSubsystem.h"
#include "GameFramework/PlayerStart.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

AEGameMode::AEGameMode()
{
	bDelayedStart = true;
	bStartPlayersAsSpectators = true;
}

void AEGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AEGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart && bHasInitializedLevelTime)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			bHasInitializedLevelTime = false;
			RestartGame();
		}
	}
}

void AEGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AEPlayerController* EPlayerController = Cast<AEPlayerController>(*It);
		if (EPlayerController)
		{
			EPlayerController->OnMatchStateSet(MatchState);
		}
	}
}

void AEGameMode::CheckAllClientsReady()
{
	ReadyClients++;

	//if (UGameInstance* GameInstance = GetGameInstance())
	//{
		//UEMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UEMultiplayerSessionsSubsystem>();
		//
		//if (!MultiplayerSessionsSubsystem) return;
		//
		//int32 CurrentPlayer = 4 - MultiplayerSessionsSubsystem->GetPlayerCount();

		if (ReadyClients == 3)
		{
			LevelStartingTime = GetWorld()->GetTimeSeconds();
			bHasInitializedLevelTime = true;

			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				if (AEPlayerController* EPlayerController = Cast<AEPlayerController>(*It))
				{
					EPlayerController->InitializeMatchState();
				}
			}
		}
	//}
}

void AEGameMode::PlayerEliminated(AEPlayer* ElimmedCharacter, AEPlayerController* VictimController, AEPlayerController* AttackerController)
{
	AEPlayerState* VictimPlayerState = VictimController ? Cast<AEPlayerState>(VictimController->PlayerState) : nullptr;
	AEPlayerState* AttackerPlayerState = AttackerController ? Cast<AEPlayerState>(AttackerController->PlayerState) : nullptr;

	AEGameState* EGameState = GetGameState<AEGameState>();

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && EGameState)
	{
		AttackerPlayerState->AddToKillScore(1.f);
		EGameState->UpdateTopScore(AttackerPlayerState);
	}
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDeathScore(1);
	}

	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void AEGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void AEGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (HasMatchStarted())
	{
		NewPlayer->StartSpectatingOnly();
	}
	else
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}



