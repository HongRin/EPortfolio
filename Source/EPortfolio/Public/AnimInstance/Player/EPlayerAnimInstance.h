// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/ECharacterAnimInstance.h"
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

private:
	void SetDirectionType();
	void SetLean(float DeltaSeconds);
	void AimOffset(float DeltaSeconds);
	void TurnInPlace(float DeltaSeconds);
	void SetLeftHandTransform();

protected :
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float DirectionAngle;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EDirectionType DirectionType;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Lean;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsJumpping;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = "Equipped")
	bool bIsEquipped;

	UPROPERTY(BlueprintReadOnly, Category = "Aiming")
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, Category = "Aiming")
	bool bIsFiring;

	UPROPERTY(BlueprintReadOnly, Category = "Aiming")
	float AimOffsetYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Aiming")
	float AimOffsetPitch;

	UPROPERTY(BlueprintReadOnly, Category = "Aiming")
	bool bIsTurn;

	UPROPERTY(BlueprintReadOnly, Category = "Layer")
	TSubclassOf<class UEPlayerLinkedAnimLayer> ItemAnimLayer;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bElimmed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bUseFABRIK;

private:
	TObjectPtr<class AEPlayer> Player;
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator StartingAimRotation;
	float    InterpAimOffsetYaw;
};
