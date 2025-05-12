// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EWeapon.h"
#include "EShotgun.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API AEShotgun : public AEWeapon
{
	GENERATED_BODY()
	
public :
	virtual void Fire(const FVector& HitTarget);

protected :
	UPROPERTY(EditDefaultsOnly, Category = "ShotgunData")
	float StartSpreadAngle;

	UPROPERTY(EditDefaultsOnly, Category = "ShotgunData")
	int32 BulletSpreadCount;
};
