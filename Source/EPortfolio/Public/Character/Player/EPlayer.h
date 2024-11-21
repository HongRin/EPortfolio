// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Character/EBaseCharacter.h"
#include "Type.h"
#include "EPlayer.generated.h"

UCLASS()
class EPORTFOLIO_API AEPlayer : public AEBaseCharacter
{
	GENERATED_BODY()
	
public :
	AEPlayer();

	// override function
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float InDeltaTime) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
public :
	UFUNCTION()
	void OnRep_OverlappingWeapon(class AEWeapon* LastWeapon);

private :
	UFUNCTION(Server, Reliable)
	void ServerEquip();

	UFUNCTION(Server, Reliable)
	void ServerSetMaxSpeed(float MaxSpeed);

public:
	void SetOverlappingWeapon(class AEWeapon* Weapon);

private :
	void AimOffset(float DeltaTime);
	void SetMaxSpeed(float MaxSpeed);
	void TurnInPlace(float DeltaTime);

private:
	void MoveAction(const struct FInputActionValue& InputActionValue);
	void LookAction(const struct FInputActionValue& InputActionValue);
	void JumpAction(const struct FInputActionValue& InputActionValue);
	void EquipAction(const struct FInputActionValue& InputActionValue);
	void CrouchAction(const struct FInputActionValue& InputActionValue);
	void RunAction(const struct FInputActionValue& InputActionValue);
	void SlowWalkAction(const struct FInputActionValue& InputActionValue);
	void AimingAction(const struct FInputActionValue& InputActionValue);

public :
	bool IsAiming();
	AEWeapon* GetEquippedWeapon();
	TSubclassOf<class UEPlayerLinkedAnimLayer> GetAnimLayer();
	FORCEINLINE float GetAimOffsetYaw() const { return AimOffsetYaw; }
	FORCEINLINE float GetAimOffsetPitch() const { return AimOffsetPitch; }
	FORCEINLINE ETurnType GetTurnType() const { return TurnType; }


public :
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<class AEWeapon> OverlappingWeapon;


	// Component
private :
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> OverheadWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UECombatComponent> CombatComponent;

private :
	float     AimOffsetYaw;
	float     AimOffsetPitch;
	FRotator  StartingAimRotation;
	ETurnType TurnType;
	float     InterpAimOffsetYaw;

	/* Input */
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> IMCPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IALook;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAJump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAEquip;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IACrouch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IARun;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IASlowWalk;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAAiming;
};
