// Copyright EPortfolio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EHUD.generated.h"


USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	TObjectPtr<class UTexture2D> CrosshairsCenter;
	TObjectPtr<class UTexture2D> CrosshairsLeft;
	TObjectPtr<class UTexture2D> CrosshairsRight;
	TObjectPtr<class UTexture2D> CrosshairsTop;
	TObjectPtr<class UTexture2D> CrosshairsBottom;
	float CrosshairSpread = 2;

public :
	void SetCrossHairTexture(UTexture2D* CenterTextrue = nullptr, UTexture2D* LeftTextrue = nullptr, UTexture2D* RightTextrue = nullptr, UTexture2D* TopTextrue = nullptr, UTexture2D* BottomTextrue = nullptr)
	{
		CrosshairsCenter = CenterTextrue;
		CrosshairsLeft   = LeftTextrue;
		CrosshairsRight  = RightTextrue;
		CrosshairsTop    = TopTextrue;
		CrosshairsBottom = BottomTextrue;
	}
};


UCLASS()
class EPORTFOLIO_API AEHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private :
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);

private:
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	
};
