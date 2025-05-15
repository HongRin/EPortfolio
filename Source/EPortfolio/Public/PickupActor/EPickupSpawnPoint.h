// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPickupSpawnPoint.generated.h"

UCLASS()
class EPORTFOLIO_API AEPickupSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPickupSpawnPoint();

protected:
	virtual void BeginPlay() override;

private :
	void SpawnPickupActor();
	void SpawnTimerFinished();

	UFUNCTION()
	void StartSpawnTimer(AActor* DestroyedActor);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayEffect();

protected :
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AEPickupActor>> PickupActorClasses;

	UPROPERTY()
	TObjectPtr<class AEPickupActor> SpawnedPickupActor;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> SpawnLoadingEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> SpawnPadEffect;

	UPROPERTY(EditAnywhere)
	float SpawnDelay;


private :
	FTimerHandle SpawnTimer;
};
