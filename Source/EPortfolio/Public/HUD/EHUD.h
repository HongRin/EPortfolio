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
protected :
	virtual void BeginPlay() override;

public :
	void SetHealthHUD(float InHealthPercent);
	void SetKillScoreHUD(float InScore);
	void SetDeathScoreHUD(int32 InScore);
	void SetAMMOHUD(int32 AMMO);
	void SetCarriedAMMOHUD(int32 AMMO);
	void SetMatchCountdownHUD(float MatchCountdown);
	void SetAnnouncementHUD(float Countdown);

	void AddAnnouncement();
	FORCEINLINE class UECharacterOverlayWidget* GetCharacterOverlay() const { return CharacterOverlay.Get(); };
	FORCEINLINE class UEAnnouncementWidget* GetAnnouncement() { return Announcement.Get(); };

private :
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);

public:
	void AddCharacterOverlay();

protected:
	FWeaponCrosshairData CrosshairData;

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	
	UPROPERTY()
	TObjectPtr<class UECharacterOverlayWidget> CharacterOverlay;


	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;

	UPROPERTY()
	TObjectPtr<class UEAnnouncementWidget> Announcement;



public:
	FORCEINLINE void SetCrosshairData(const FWeaponCrosshairData & InWeaponCrossHairData)
	{
		CrosshairData = InWeaponCrossHairData;
	}
};
