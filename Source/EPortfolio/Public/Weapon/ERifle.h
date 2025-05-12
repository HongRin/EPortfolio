// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EWeapon.h"
#include "ERifle.generated.h"

UCLASS()
class EPORTFOLIO_API AERifle: public AEWeapon
{
	GENERATED_BODY()
	
public :
	virtual void Fire(const FVector& HitTarget) override;
};
