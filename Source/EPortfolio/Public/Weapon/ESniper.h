// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EWeapon.h"
#include "ESniper.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API AESniper : public AEWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Fire(const FVector& HitTarget);

};
