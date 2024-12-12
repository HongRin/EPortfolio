// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Weapon/FWeaponDatas.h"
#include "EHUD.generated.h"



UCLASS()
class EPORTFOLIO_API AEHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private :
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);

private:
	FWeaponCrosshairData CrosshairData;

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

public:
	FORCEINLINE void SetCrosshairData(const FWeaponCrosshairData & InWeaponCrossHairData)
	{
		CrosshairData = InWeaponCrossHairData;
	}
};
