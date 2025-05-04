// Copyright EPortfolio


#include "Game/EPlayerState.h"
#include "Character/Player/EPlayer.h"
#include "Controller/EPlayerController.h"
#include "Net/UnrealNetwork.h"

void AEPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEPlayerState, DeathScore);
}

void AEPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Player = Player == nullptr ? Cast<AEPlayer>(GetPawn()) : Player.Get();

	if (Player)
	{
		PlayerController = PlayerController == nullptr ? Cast<AEPlayerController>(Player->Controller) : PlayerController.Get();
		if (PlayerController)
		{
			PlayerController->UpdateKillScoreHUD(GetScore());
		}			
	}
}

void AEPlayerState::AddToKillScore(float ScoreVaule)
{
	SetScore(GetScore() + ScoreVaule);

	Player = Player == nullptr ? Cast<AEPlayer>(GetPawn()) : Player.Get();

	if (Player)
	{
		PlayerController = PlayerController == nullptr ? Cast<AEPlayerController>(Player->Controller) : PlayerController.Get();
		if (PlayerController)
		{
			PlayerController->UpdateKillScoreHUD(GetScore());
		}
	}
}

void AEPlayerState::AddToDeathScore(int32 ScoreVaule)
{
	DeathScore += ScoreVaule;

	Player = Player == nullptr ? Cast<AEPlayer>(GetPawn()) : Player.Get();

	if (Player)
	{
		PlayerController = PlayerController == nullptr ? Cast<AEPlayerController>(Player->Controller) : PlayerController.Get();
		if (PlayerController)
		{
			PlayerController->UpdateDeathScoreHUD(DeathScore);
		}
	}
}

void AEPlayerState::OnRep_DeathScore()
{
	Player = Player == nullptr ? Cast<AEPlayer>(GetPawn()) : Player.Get();

	if (Player)
	{
		PlayerController = PlayerController == nullptr ? Cast<AEPlayerController>(Player->Controller) : PlayerController.Get();
		if (PlayerController)
		{
			PlayerController->UpdateDeathScoreHUD(DeathScore);
		}
	}
}
