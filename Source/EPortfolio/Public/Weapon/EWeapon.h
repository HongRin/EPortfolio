// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	WS_Initial  UMETA(DisplayName = "Initial State"),
	WS_Equipped UMETA(DisplayName = "Equipped"),
	WS_Dropped  UMETA(DisplayName = "Dropped"),
	WS_MAX      UMETA(DisplayName = "DefaultMAX")
};

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

public :
	void ShowPickupWidget(bool bShowWidget);
	void SetWeaponState(EWeaponState State);
	virtual void Fire(const FVector& HitTarget);

protected :
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_WeaponState();

public :
	FORCEINLINE class USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE TSubclassOf<class UEPlayerLinkedAnimLayer> GetWeaponAnimLayerClass() { return WeaponAnimLayerClass; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE UAnimMontage* GetWeaponFireMontage() const { return WeaponFireMontage; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }



protected :
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class USphereComponent> AreaSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UWidgetComponent> PickupWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TSubclassOf<class UEPlayerLinkedAnimLayer> WeaponAnimLayerClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TSubclassOf<class AECasing> CasingClass;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> WeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimationAsset> WeaponFireAnimantion;

	UPROPERTY(EditDefaultsOnly)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditDefaultsOnly)
	float ZoomInterpSpeed = 20.f;

public :
	UPROPERTY(EditDefaultsOnly, Category = Crosshairs)
	TObjectPtr<class UTexture2D> CrosshairsCenter;

	UPROPERTY(EditDefaultsOnly, Category = Crosshairs)
	TObjectPtr<class UTexture2D> CrosshairsLeft;

	UPROPERTY(EditDefaultsOnly, Category = Crosshairs)
	TObjectPtr<class UTexture2D> CrosshairsRight;

	UPROPERTY(EditDefaultsOnly, Category = Crosshairs)
	TObjectPtr<class UTexture2D> CrosshairsTop;

	UPROPERTY(EditDefaultsOnly, Category = Crosshairs)
	TObjectPtr<class UTexture2D> CrosshairsBottom;

	


};
