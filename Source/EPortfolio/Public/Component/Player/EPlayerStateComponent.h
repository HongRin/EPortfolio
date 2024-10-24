// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "Component/EStateComponent.h"
#include "EPlayerStateComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EPORTFOLIO_API UEPlayerStateComponent : public UEStateComponent
{
	GENERATED_BODY()
	
public :
	virtual void BeginPlay() override;

public :
	void SetJump();

private :
	UFUNCTION()
	void DisabledJumpping(const FHitResult& Hit);

public:
	FORCEINLINE bool IsJumpping() const { return Jumpping; }
	FORCEINLINE bool IsCanMove() const  { return bCanMove; }
	FORCEINLINE void EnableCanMove()    { bCanMove = true; };
	FORCEINLINE void DisableCanMove()   { bCanMove = false; };


private:
	bool Jumpping { false };
	bool bCanMove { true };

private :
	TObjectPtr<class AEPlayer> OwnerPlayer;
};
