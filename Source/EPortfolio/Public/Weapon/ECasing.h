// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ECasing.generated.h"

UCLASS()
class EPORTFOLIO_API AECasing : public AActor
{
	GENERATED_BODY()
	
public:	
	AECasing();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> CasingMesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;
};
