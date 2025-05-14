// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ELobbyGameState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerCountChanged, int32, NewCount);

UCLASS()
class EPORTFOLIO_API AELobbyGameState : public AGameState
{
	GENERATED_BODY()
	

public :
    void UpdatePlayerCount(int32 Count);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY()
    FOnPlayerCountChanged OnPlayerCountChanged;

    FORCEINLINE int32 GetCurrentPlayers() const { return CurrentPlayers; }

private:
    UFUNCTION()
    void OnRep_PlayerCount();

private:
    UPROPERTY(ReplicatedUsing = OnRep_PlayerCount)
    int32 CurrentPlayers;
};
