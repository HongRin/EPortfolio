// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "FWeaponDatas.generated.h" 

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	WS_Initial  UMETA(DisplayName = "Initial State"),
	WS_Equipped UMETA(DisplayName = "Equipped"),
	WS_Dropped  UMETA(DisplayName = "Dropped"),
	WS_MAX      UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_Rifle    UMETA(DisplayName = "Rifle"),
	WT_MAX      UMETA(DisplayName = "Default Max"),
};


USTRUCT(Atomic, BlueprintType)
struct FEWeaponAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float TargetArmLength;

	UPROPERTY(EditDefaultsOnly)
	FVector SocketOffset;

	UPROPERTY(EditDefaultsOnly)
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly)
	bool bEnableCameraLag;
};

USTRUCT(Atomic, BlueprintType)
struct FEWeaponAnimData
{
	GENERATED_BODY()
public :
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> WeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimationAsset> WeaponFireAnimantion;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> WeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimationAsset> WeaponReloadAnimantion;
};

USTRUCT(Atomic, BlueprintType)
struct FEWeaponClassData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TSubclassOf<class UEPlayerLinkedAnimLayer> WeaponAnimLayerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	TSubclassOf<class AECasing> CasingClass;
};

USTRUCT(Atomic, BlueprintType)
struct FWeaponCrosshairData
{
	GENERATED_BODY()
public:
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

	UPROPERTY(EditDefaultsOnly, Category = Crosshairs)
	float CrosshairSpread;

public :
	void SetCrosshairTexture(UTexture2D* Center = nullptr, UTexture2D* Left = nullptr, UTexture2D* Right = nullptr, UTexture2D* Top = nullptr, UTexture2D* Bottom = nullptr)
	{
		CrosshairsCenter = Center;
		CrosshairsLeft   = Left;
		CrosshairsRight  = Right;
		CrosshairsTop    = Top;
		CrosshairsBottom = Bottom;
	}
};


USTRUCT(Atomic, BlueprintType)
struct FWeaponDatas
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FEWeaponAimData AimDatas;

	UPROPERTY(EditAnywhere)
	FEWeaponAimData BaseDatas;

	UPROPERTY(EditAnywhere)
	FEWeaponAnimData AnimDatas;

	UPROPERTY(EditAnywhere)
	FEWeaponClassData ClassDatas;

	UPROPERTY(EditAnywhere)
	FWeaponCrosshairData CrosshairDatas;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	TObjectPtr<class UCurveFloat> AimCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	float AimingSpeed = 200;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float RecoilRate;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float AutoFireInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TSubclassOf<class ULegacyCameraShake> CameraShakeClass;
};

