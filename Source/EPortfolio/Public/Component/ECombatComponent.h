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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

public :
	void EquipWeapon(class AEWeapon* WeaponToEquip);

protected :
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	void SetItemAnimLayer();

	UFUNCTION(Server, Reliable)
	void ServerSetItemAnimLayer();

protected :
	UPROPERTY(EditDefaultsOnly, Category = "AnimLayer")
	TSubclassOf<class UEPlayerLinkedAnimLayer> UnarmedAnimLayer;

private:
	TObjectPtr<class AEPlayer> Player;

	UPROPERTY(Replicated)
	TObjectPtr<class AEWeapon> EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(Replicated)
	TSubclassOf<class UEPlayerLinkedAnimLayer> ItemAnimLayer;
};
