// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "PickupActor/EPickupActor.h"
#include "EPickupShieldpack.generated.h"

UCLASS()
class EPORTFOLIO_API AEPickupShieldpack : public AEPickupActor
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	float ShieldAmount = 100.f;
};
