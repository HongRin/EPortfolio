// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EGameMode.generated.h"

namespace MatchState
{
	extern EPORTFOLIO_API const FName Cooldown;
}


UCLASS()
class EPORTFOLIO_API AEGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AEGameMode();
	virtual void Tick(float DeltaTime) override;

	virtual void PlayerEliminated(class AEPlayer* ElimmedCharacter, class AEPlayerController* VictimController, AEPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;

public :
	void CheckAllClientsReady();

public:
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 15.f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 5.f;

	float LevelStartingTime = 0.f;

private:
	float CountdownTime = 0.f;

	bool bHasInitializedLevelTime = false;
	int32 ReadyClients = 0;
};
