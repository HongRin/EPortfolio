// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "PickupActor/EPickupActor.h"
#include "EPickupHealthpack.generated.h"

/**
 * 
 */
UCLASS()
class EPORTFOLIO_API AEPickupHealthpack : public AEPickupActor
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected :
	UPROPERTY(EditAnywhere)
	float HealAmount;
};
