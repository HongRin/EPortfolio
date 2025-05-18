// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/FWeaponDatas.h"
#include "Type/ECombatState.h"
#include "ECombatComponent.generated.h"

USTRUCT(BlueprintType)
struct FDirectionMontages
{
	GENERATED_BODY()
public :
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> Left;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> Forward;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> Right;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> Backward;
};

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
	void PlayHitReactMontage(const FVector2D Direction);
	void PlayDodgeMontage(const FVector2D Direction);
	void FinsishedReload();
	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

private :
	UFUNCTION()
	void OnRep_EquippedWeapon();

	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetFiring(bool bIsFiring);

	void SetItemAnimLayer();

	UFUNCTION(Server, Reliable)
	void ServerSetItemAnimLayer();

	void Firing(bool bFireButtonPressed);

	void OnFiring();
	void FiringTimerFunction();

	UFUNCTION(Server, Reliable)
	void ServerFiring(const FVector_NetQuantize& TraceHitTarget);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFiring(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairsSpread(float DeltaTime);

	bool CanFire();

	UFUNCTION()
	void OnRep_CarriedAmmo();

	void Reload();

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void InitializeCarriedAmmo();

	void ReloadHandle();

	int32 AmountToReload();

	UFUNCTION()
	void OnRep_CombatState();

	void UpdateAmmoValues();

	void SetAimMouseSensitivity();

protected :
	UPROPERTY(EditDefaultsOnly, Category = "AnimLayer")
	TSubclassOf<class UEPlayerLinkedAnimLayer> UnarmedAnimLayer;

private:
	UPROPERTY()
	TObjectPtr<class AEPlayer> Player;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<class AEWeapon> EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(Replicated)
	bool bFiring;

	UPROPERTY(Replicated)
	TSubclassOf<class UEPlayerLinkedAnimLayer> ItemAnimLayer;

	UPROPERTY()
	TObjectPtr<class AEPlayerController> Controller;
	
	UPROPERTY()
	TObjectPtr<class AEHUD> HUD;

	float CrosshairBaseFactor;
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootFactor;
	float HitDistance;

	FVector HitTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	FDirectionMontages HitReactMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	FDirectionMontages DodgeMontages;

	FTimerHandle FireTimer;
	bool bCanFire;

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	TMap<EWeaponType, int32> MaxCarriedAmmoMap;

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState;

	float MouseSensitivity = 0.8f;
};
