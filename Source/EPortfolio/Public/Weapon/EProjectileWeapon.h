// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Weapon/EWeapon.h"
#include "EProjectileWeapon.generated.h"

UCLASS()
class EPORTFOLIO_API AEProjectileWeapon : public AEWeapon
{
	GENERATED_BODY()
	
public :
	virtual void Fire(const FVector& HitTarget) override;

protected :
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEProjectile> ProjectileClass;
};
