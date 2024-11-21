// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/ECharacterAnimInstance.h"
#include "Type.h"
#include "EPlayerAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EDirectionType : uint8
{
	Forward,
	Backward,
	Right,
	Left,
};

UCLASS()
class EPORTFOLIO_API UEPlayerAnimInstance : public UECharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected :
	TObjectPtr<class AEWeapon> EquippedWeapon;
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float DirectionAngle;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	EDirectionType DirectionType;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Lean;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsJumpping;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = "Equipped")
	bool bIsEquipped;

	UPROPERTY(BlueprintReadOnly, Category = "Aiming")
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, Category = "Layer")
	TSubclassOf<class UEPlayerLinkedAnimLayer> ItemAnimLayer;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float AimOffsetYaw;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float AimOffsetPitch;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	ETurnType TurnType;
};
