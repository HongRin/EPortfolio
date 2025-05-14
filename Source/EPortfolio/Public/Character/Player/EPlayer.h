// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Character/EBaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "Type/ECombatState.h"
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
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
public :
	UFUNCTION()
	void OnRep_OverlappingWeapon(class AEWeapon* LastWeapon);

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHit(FVector2D Direction);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

	void Elim();

private :
	UFUNCTION(Server, Reliable)
	void ServerEquip();

	UFUNCTION(Server, Reliable)
	void ServerSetMaxSpeed(float MaxSpeed);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetMaxSpeed(float MaxSpeed);

public:
	void SetOverlappingWeapon(class AEWeapon* Weapon);

private :
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	void UpdateHealth();

	void ElimTimerFinished();

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	void StartDissolve();

	void PollInit();

	void SetMaxSpeed(float MaxSpeed);


private:
	void MoveAction(const struct FInputActionValue& InputActionValue);
	void LookAction(const struct FInputActionValue& InputActionValue);
	void JumpAction(const struct FInputActionValue& InputActionValue);
	void EquipAction(const struct FInputActionValue& InputActionValue);
	void CrouchAction(const struct FInputActionValue& InputActionValue);
	void RunAction(const struct FInputActionValue& InputActionValue);
	void SlowWalkAction(const struct FInputActionValue& InputActionValue);
	void AimingAction(const struct FInputActionValue& InputActionValue);
	void FiringAction(const struct FInputActionValue& InputActionValue);
	void ReloadAction(const struct FInputActionValue& InputActionValue);
	void SystemMenuAction(const struct FInputActionValue& InputActionValue);


public :
	bool IsAiming();
	bool IsFiring();
	AEWeapon* GetEquippedWeapon();
	TSubclassOf<class UEPlayerLinkedAnimLayer> GetAnimLayer();
	FVector GetHitTarget() const;
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; };
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; };
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	float GetFootStepVolume() const;
	FORCEINLINE float GetHealth()    const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	ECombatState GetCombatState() const;


public :
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<class AEWeapon> OverlappingWeapon;

	// Component
private :
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UECombatComponent> CombatComponent;

private :
	UPROPERTY()
	TObjectPtr<class AEPlayerController> EPlayerController;
	bool bElimmed = false;

	FTimerHandle ElimTimer;

	FOnTimelineFloat DissolveTrack;

	UPROPERTY()
	TObjectPtr<class AEPlayerState> EPlayerState;

protected :
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float CrouchSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SlowWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

protected :
	UPROPERTY(EditDefaultsOnly, Category = "Elim")
	UAnimMontage* ElimMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTimelineComponent> DissolveTimeline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> DissolveCurve;

	UPROPERTY(VisibleAnywhere, Category = "Elim")
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicDissolveMaterialInstances;

	UPROPERTY(EditAnywhere, Category = "Elim")
	TArray<TObjectPtr<UMaterialInstance>> DissolveMaterialInstances;

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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAFire;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAReload;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IASystemMenu;
};
