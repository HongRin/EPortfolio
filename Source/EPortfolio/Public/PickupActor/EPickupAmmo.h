// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "PickupActor/EPickupActor.h"
#include "Weapon/FWeaponDatas.h"
#include "EPickupAmmo.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API AEPickupAmmo : public AEPickupActor
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	int32 AmmoAmount = 30;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
};
