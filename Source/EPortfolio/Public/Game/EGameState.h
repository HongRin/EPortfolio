// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EGameState.generated.h"

UCLASS()
class EPORTFOLIO_API AEGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void UpdateTopScore(class AEPlayerState* ScoringPlayer);
	UPROPERTY(Replicated)
	TArray<AEPlayerState*> TopScoringPlayers;
private:
	float TopScore = 0.f;

};
