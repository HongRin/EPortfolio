// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EProjectile.generated.h"

UCLASS()
class EPORTFOLIO_API AEProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AEProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected :
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USphereComponent> ProjecttileCollision;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> ProjecttileMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UParticleSystem> ImpactParticles;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USoundCue> ImpactSound;


};
