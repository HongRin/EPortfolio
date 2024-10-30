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

protected :
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_WeaponState();

public :
		FORCEINLINE class USphereComponent* GetAreaSphere() const { return AreaSphere; }

protected :
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class USphereComponent> AreaSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TObjectPtr<class UWidgetComponent> PickupWidget;

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;
};
