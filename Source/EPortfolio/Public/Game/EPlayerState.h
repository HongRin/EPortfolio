// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API AEPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public :
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void OnRep_Score() override;
	
public :
	void AddToKillScore(float ScoreVaule);
	void AddToDeathScore(int32 ScoreVaule);

private :
	UFUNCTION()
	virtual void OnRep_DeathScore();

private :
	UPROPERTY()
	TObjectPtr<class AEPlayer> Player;
	UPROPERTY()
	TObjectPtr<class AEPlayerController> PlayerController;

	UPROPERTY(ReplicatedUsing = OnRep_DeathScore)
	int32 DeathScore;
};
