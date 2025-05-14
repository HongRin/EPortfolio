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
	void SetAmmoHUD(int32 Ammo);
	void SetCarriedAmmoHUD(int32 Ammo);
	void SetMatchCountdownHUD(float MatchCountdown);
	void SetAnnouncementHUD(float Countdown);
	void SetSniperScopeWidgetHUD(bool bIsAiming);

	void AddAnnouncement();
	void AddCharacterOverlay();
	void AddSniperScopeWidget();

	FORCEINLINE class UECharacterOverlayWidget* GetCharacterOverlay() const { return CharacterOverlay.Get(); };
	FORCEINLINE class UEAnnouncementWidget* GetAnnouncement() { return Announcement.Get(); };
	FORCEINLINE class UESniperScopeWidget* GetSniperScopeWidget() { return SniperScopeWidget.Get(); };


private :
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);


protected:
	FWeaponCrosshairData CrosshairData;

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	
	UPROPERTY()
	TObjectPtr<class UECharacterOverlayWidget> CharacterOverlay;

	UPROPERTY(EditAnywhere, Category = "Sniper Scope")
	TSubclassOf<class UUserWidget> SniperScopeWidgetClass;

	UPROPERTY()
	TObjectPtr<class UESniperScopeWidget> SniperScopeWidget;

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
