// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API AEPlayerController : public APlayerController
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

public :
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual float GetServerTime();
	virtual void ReceivedPlayer() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void UpdateHealthHUD(float Health, float MaxHealth);
	void UpdateShieldHUD(float Shield, float MaxShield);
	void UpdateKillScoreHUD(float Score);
	void UpdateDeathScoreHUD(int32 Score);
	void UpdateAmmoHUD(int32 Ammo);
	void UpdateCarriedAmmoHUD(int32 Ammo);
	void UpdateMatchCountdownHUD(float CountdownTime);
	void UpdateAnnouncementHUD(float CountdownTime);
	void UpdateSniperScopeHUD(bool bIsAiming);
	void OnMatchStateSet(FName State);
	void InitializeMatchState();
	void CheckClientReady();
	
public :
	void ToggleSystemMenu();

private :
	UFUNCTION(Server, Reliable)
	void ServerCheckClientReady();
	
	void SetHUDTime();

	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);
	void CheckTimeSync(float DeltaTime);

	UFUNCTION()
	void OnRep_MatchState();

	void PollInit();

	void HandleMatchHasStarted();

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

	void HandleCooldown();



private:
	UPROPERTY()
	TObjectPtr<class AEHUD> PlayerHUD;

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;
	float WarmupTime = 0.f;
	float CooldownTime = 0.f;
	uint32 Countdown = 0;

	float ClientServerDelta = 0.f; 

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;
	
	UPROPERTY()
	TObjectPtr<class UECharacterOverlayWidget> CharacterOverlay;

	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;
	int32 HUDDefeats;
	float HUDShield;
	float HUDMaxShield;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<class UUserWidget> SystemMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<class UESystemMenuWidget> SystemMenu;
};
