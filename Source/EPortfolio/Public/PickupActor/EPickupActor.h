// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPickupActor.generated.h"

UCLASS()
class EPORTFOLIO_API AEPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPickupActor();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private :
	void OverlapTimerFinished();


protected :
	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;

protected :
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> OverlapSphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> PickupSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> PickupEffect;

	TObjectPtr<class AEPlayer> Player;


	FTimerHandle OverlapTimer;
	float OverlapTime = 0.25f;
};
