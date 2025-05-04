// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ELobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API AELobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
