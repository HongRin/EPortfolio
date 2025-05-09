// Copyright EPortfolio


#include "Controller/EPlayerController.h"
#include "Character/Player/EPlayer.h"
#include "Net/UnrealNetwork.h"
#include "HUD/EHUD.h"
#include "HUD/EAnnouncementWidget.h"
#include "HUD/ECharacterOverlayWidget.h"
#include "Game/EGameMode.h"
#include "Game/EPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Game/EGameState.h"

#include "Game/ELobbyGameMode.h"
#include "ELogHelpers.h"

void AEPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = Cast<AEHUD>(GetHUD());
}

void AEPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
}

void AEPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEPlayer* PlayerCharacter = Cast<AEPlayer>(InPawn);
	if (PlayerCharacter)
	{
		UpdateHealthHUD(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
	}
}

float AEPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AEPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void AEPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEPlayerController, MatchState);
}

void AEPlayerController::UpdateHealthHUD(float InHealth, float InMaxHealth)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();

	if (PlayerHUD)
	{
		PlayerHUD->SetHealthHUD(InHealth / InMaxHealth);
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDHealth    = InHealth;
		HUDMaxHealth = InMaxHealth;
	}
}

void AEPlayerController::UpdateKillScoreHUD(float Score)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();

	if (PlayerHUD)
	{
		PlayerHUD->SetKillScoreHUD(Score);
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDScore = Score;
	}
}


void AEPlayerController::UpdateDeathScoreHUD(int32 Score)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();

	if (PlayerHUD)
	{
		PlayerHUD->SetDeathScoreHUD(Score);
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDDefeats = Score;
	}
}

void AEPlayerController::UpdateAMMOHUD(int32 AMMO)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();

	if (PlayerHUD)
	{
		PlayerHUD->SetAMMOHUD(AMMO);
	}
}

void AEPlayerController::UpdateCarriedAMMOHUD(int32 AMMO)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();

	if (PlayerHUD)
	{
		PlayerHUD->SetCarriedAMMOHUD(AMMO);
	}
}

void AEPlayerController::UpdateMatchCountdownHUD(float CountdownTime)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();
	if (PlayerHUD)
	{
		PlayerHUD->SetMatchCountdownHUD(CountdownTime);
	}
}

void AEPlayerController::UpdateAnnouncementHUD(float CountdownTime)
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();

	if (PlayerHUD)
	{
		PlayerHUD->SetAnnouncementHUD(CountdownTime);
	}
}

void AEPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void AEPlayerController::InitializeMatchState()
{
	ServerCheckMatchState();
}

void AEPlayerController::CheckClientReady()
{
	if (IsLocalController())
	{
		ServerCheckClientReady();
	}
}

void AEPlayerController::ServerCheckClientReady_Implementation()
{
	if (AEGameMode* GameMode = Cast<AEGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->CheckAllClientsReady();
	}
}

void AEPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (Countdown != SecondsLeft)
	{
		if (MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			UpdateAnnouncementHUD(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			UpdateMatchCountdownHUD(TimeLeft);
		}
	}

	Countdown = SecondsLeft;
}

void AEPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AEPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void AEPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void AEPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}


void AEPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (PlayerHUD && PlayerHUD->GetCharacterOverlay())
		{
			CharacterOverlay = PlayerHUD->GetCharacterOverlay();
			if (CharacterOverlay)
			{
				UpdateHealthHUD(HUDHealth, HUDMaxHealth);
				UpdateKillScoreHUD(HUDScore);
				UpdateDeathScoreHUD(HUDDefeats);
			}
		}
	}
}

void AEPlayerController::HandleMatchHasStarted()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();
	if (PlayerHUD)
	{
		if (PlayerHUD->GetCharacterOverlay() == nullptr)
		{
			PlayerHUD->AddCharacterOverlay();
		}

		if (PlayerHUD->GetAnnouncement())
		{
			PlayerHUD->GetAnnouncement()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
void AEPlayerController::HandleCooldown()
{
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();

	if (PlayerHUD)
	{
		PlayerHUD->GetCharacterOverlay()->RemoveFromParent();
		if (PlayerHUD->GetAnnouncement())
		{
			PlayerHUD->GetAnnouncement()->SetVisibility(ESlateVisibility::Visible);

			FString AnnouncementText("New Match Start");
			PlayerHUD->GetAnnouncement()->TextBlock_Announcement->SetText(FText::FromString(AnnouncementText));
			PlayerHUD->GetAnnouncement()->TextBlock_Info->SetText(FText());

			AEGameState* EGameState = Cast<AEGameState>(UGameplayStatics::GetGameState(this));
			AEPlayerState* EPlayerState = GetPlayerState<AEPlayerState>();
			if (EGameState && PlayerState)
			{
				TArray<AEPlayerState*> TopPlayers = EGameState->TopScoringPlayers;
				FString InfoTextString;
				if (TopPlayers.Num() == 0)
				{
					InfoTextString = FString("There is no winner.");
				}
				else if (TopPlayers.Num() == 1 && TopPlayers[0] == EPlayerState)
				{
					InfoTextString = FString("You are the winner!");
				}
				else if (TopPlayers.Num() == 1)
				{
					InfoTextString = FString::Printf(TEXT("Winner \n%s"), *TopPlayers[0]->GetPlayerName());
				}
				else if (TopPlayers.Num() > 1)
				{
					InfoTextString = FString("Players tied for the win\n");
					for (auto TiedPlayer : TopPlayers)
					{
						InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
					}
				}
				PlayerHUD->GetAnnouncement()->TextBlock_Info->SetText(FText::FromString(InfoTextString));
			}
		}
	}
}

void AEPlayerController::ServerCheckMatchState_Implementation()
{
	if (AEGameMode* GameMode = Cast<AEGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();

 		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

void AEPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
	
	PlayerHUD = PlayerHUD == nullptr ? Cast<AEHUD>(GetHUD()) : PlayerHUD.Get();

	if (PlayerHUD && (MatchState == MatchState::WaitingToStart || MatchState == MatchState::EnteringMap))
	{
		PlayerHUD->AddAnnouncement();
	}
}
