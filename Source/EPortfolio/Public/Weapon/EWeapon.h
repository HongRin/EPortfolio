// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FWeaponDatas.h"
#include "EWeapon.generated.h"


UCLASS()
class EPORTFOLIO_API AEWeapon : public AActor
{
	GENERATED_BODY()

public:
	AEWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void ShowPickupWidget(bool bShowWidget);
	void SetWeaponState(EWeaponState State);
	virtual void Fire(const FVector& HitTarget);
	void Aimimg(bool bAiming);
	void DrawDecal(const FVector InLocation, const FRotator InRotator);
	void Dropped();
	virtual void OnRep_Owner() override;
	void SetHUDAMMO();
	bool IsAMMOEmpty();
	void AddAMMO(int32 AMMOtoAdd);
	void Reload();

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_WeaponState();

	UFUNCTION()
	void OnAiming(float Output);

	UFUNCTION()
	void OnRep_AMMO();


private :
	void SetAimData(const FEWeaponAimData & InWeaponAimData);
	void SpendAMMO();


public:
	FORCEINLINE class USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE TSubclassOf<class UEPlayerLinkedAnimLayer> GetWeaponAnimLayerClass() { return WeaponDatas.ClassDatas.WeaponAnimLayerClass; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE UAnimMontage* GetWeaponFireMontage() const { return WeaponDatas.AnimDatas.WeaponFireMontage; }
	FORCEINLINE UAnimMontage* GetWeaponReloadMontage() const { return WeaponDatas.AnimDatas.WeaponReloadMontage; }
	FORCEINLINE const FWeaponCrosshairData& GetWeaponCrosshairData() const { return WeaponDatas.CrosshairDatas; }
	FORCEINLINE const float GetAutoFireInterval() const { return WeaponDatas.AutoFireInterval; }
	FORCEINLINE const bool GetAutomatic() const { return bAutomatic; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE int32 GetAMMO() const { return AMMO; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

protected :
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class USphereComponent> AreaSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UWidgetComponent> PickupWidget;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	FWeaponDatas WeaponDatas;

	UPROPERTY(EditDefaultsOnly, Category = "Timeline")
	TObjectPtr<class UTimelineComponent> Timeline;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	TObjectPtr<class UMaterialInstanceConstant> HitDecal;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	bool bAutomatic;

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Ammo, Category = "AMMO")
	int32 AMMO;

	UPROPERTY(EditDefaultsOnly, Category = "AMMO")
	int32 MagazineCapacity;

	UPROPERTY()
	TObjectPtr<class AEPlayer> OwnerCharacter;
	UPROPERTY()
	TObjectPtr<class AEPlayerController>  OwnerController;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	EWeaponType WeaponType;
};
