// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EBuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EPORTFOLIO_API UEBuffComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AEPlayer;

public:	
	UEBuffComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public :
	void  PlayPickupEffect(class UNiagaraSystem* PickupEffect);

	void ReplenishShield(float InShieldAmount);

	void Heal(float InHealAmount);

private :
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPickupEffect(class UNiagaraSystem* PickupEffect);

	void ShieldRecovery(float DeltaTime);
	void HealthRecovery(float DeltaTime);


private :
	UPROPERTY()
	TObjectPtr<class AEPlayer> Player;

	bool bReplenishingShield = false;
	float ShieldReplenishRate = 0.f;
	float ShieldAmount = 0.f;

	bool bHealing = false;
	float HealingRate = 0.f;
	float HealAmount = 0.f;



protected:
	UPROPERTY(EditAnywhere)
	float ShieldReplenishTime = 3.f;

	UPROPERTY(EditAnywhere)
	float HealingTime = 3.f;
};
