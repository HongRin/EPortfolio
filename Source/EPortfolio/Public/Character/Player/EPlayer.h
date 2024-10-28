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

public :
	virtual void BeginPlay() override;
	virtual void Tick(float InDeltaTime) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :
	virtual void Landed(const FHitResult& Hit) override;

private:
	void MoveAction(const struct FInputActionValue& InputActionValue);
	void LookAction(const struct FInputActionValue& InputActionValue);
	void JumpAction(const struct FInputActionValue& InputActionValue);

public :
	FORCEINLINE class UEPlayerStateComponent* GetStateComponent() { return StateComponent; }

private :
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> OverheadWidget;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IALook;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> IAJump;


protected :
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> IMCPlayer;

private :
	TObjectPtr<class UEPlayerStateComponent> StateComponent;

};
