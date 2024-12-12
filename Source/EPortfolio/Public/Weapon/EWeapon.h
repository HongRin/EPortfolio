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

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_WeaponState();

	UFUNCTION()
	void OnAiming(float Output);

private :
	void SetAimData(const FEWeaponAimData & InWeaponAimData);


public:
	FORCEINLINE class USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE TSubclassOf<class UEPlayerLinkedAnimLayer> GetWeaponAnimLayerClass() { return WeaponDatas.ClassDatas.WeaponAnimLayerClass; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE UAnimMontage* GetWeaponFireMontage() const { return WeaponDatas.AnimDatas.WeaponFireMontage; }
	FORCEINLINE const FWeaponCrosshairData& GetWeaponCrosshairData() const { return WeaponDatas.CrosshairDatas; }
	FORCEINLINE const float GetAutoFireInterval() const { return WeaponDatas.AutoFireInterval; }
	FORCEINLINE const bool GetAutomatic() const { return bAutomatic; }



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

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	bool bAutomatic;
};
