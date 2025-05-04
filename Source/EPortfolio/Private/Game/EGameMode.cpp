// Copyright EPortfolio


#include "Game/EGameMode.h"
#include "Character/Player/EPlayer.h"
#include "Controller/EPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/EPlayerState.h"
#include "Game/EGameState.h"
#include "GameFramework/PlayerStart.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

AEGameMode::AEGameMode()
{
	bDelayedStart = true;
	//StartPlay();
}

void AEGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
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
			RestartGame();
		}
	}
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

void AEGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
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
