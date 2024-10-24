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
	void Move(const FVector2D & InMovementVector, const FRotator& MovementRotation);
	void Look(const double & InX, const double & InY);
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

public :
	// FORCEINLINE class UEPlayerStateComponent* GetPlayerState() { return PlayerState; }
	class UEPlayerStateComponent* GetPlayerState() { return PlayerState; }

private :
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

private :
	TObjectPtr<class UEPlayerStateComponent> PlayerState;


};
