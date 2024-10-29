// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Character/EArmedCharacter.h"
#include "EPlayer.generated.h"

UCLASS()
class EPORTFOLIO_API AEPlayer : public AEArmedCharacter
{
	GENERATED_BODY()
	
public :
	AEPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float InDeltaTime) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public :
	virtual void Landed(const FHitResult& Hit) override;

public :
	UFUNCTION()
	void OnRep_OverlappingWeapon(class AEWeapon* LastWeapon);

public:
	void SetOverlappingWeapon(class AEWeapon* Weapon);

private:
	void MoveAction(const struct FInputActionValue& InputActionValue);
	void LookAction(const struct FInputActionValue& InputActionValue);
	void JumpAction(const struct FInputActionValue& InputActionValue);

public :
	FORCEINLINE class UEPlayerStateComponent* GetStateComponent() { return StateComponent; }

public :
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<class AEWeapon> OverlappingWeapon;

private :
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> OverheadWidget;

	TObjectPtr<class UEPlayerStateComponent> StateComponent;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> IMCPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IALook;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAJump;
};
