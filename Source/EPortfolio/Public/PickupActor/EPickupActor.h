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
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(EditAnywhere)
	class USphereComponent* OverlapSphere;

	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

};
