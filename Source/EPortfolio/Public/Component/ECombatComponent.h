// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ECombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EPORTFOLIO_API UECombatComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AEPlayer;

public:
	UECombatComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public :
	void EquipWeapon(class AEWeapon* WeaponToEquip);

private:
	TObjectPtr<class AEPlayer> Player;
	TObjectPtr<class AEWeapon> EquippedWeapon;
};
