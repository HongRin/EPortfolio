// Copyright EPortfolio


#include "HUD/EHUD.h"
#include "GameFramework/PlayerController.h"
#include "HUD/ECharacterOverlayWidget.h"
#include "HUD/EAnnouncementWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void AEHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrosshairSpreadMax * CrosshairData.CrosshairSpread;

		if (CrosshairData.CrosshairsCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(CrosshairData.CrosshairsCenter, ViewportCenter, Spread);
		}
		if (CrosshairData.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(CrosshairData.CrosshairsLeft, ViewportCenter, Spread);
		}
		if (CrosshairData.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(CrosshairData.CrosshairsRight, ViewportCenter, Spread);
		}
		if (CrosshairData.CrosshairsTop)
		{
			FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshair(CrosshairData.CrosshairsTop, ViewportCenter, Spread);
		}
		if (CrosshairData.CrosshairsBottom)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(CrosshairData.CrosshairsBottom, ViewportCenter, Spread);
		}
	}
}

void AEHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AEHUD::SetHealthHUD(float InHealthPercent)
{
	if (CharacterOverlay && CharacterOverlay->ProgressBar_Health)
	{
		CharacterOverlay->ProgressBar_Health->SetPercent(InHealthPercent);
	}
}

void AEHUD::SetKillScoreHUD(float InScore)
{
	if (CharacterOverlay && CharacterOverlay->TextBlock_KillScore)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(InScore));
		CharacterOverlay->TextBlock_KillScore->SetText(FText::FromString(ScoreText));
	}
}

void AEHUD::SetDeathScoreHUD(int32 InScore)
{
	if (CharacterOverlay && CharacterOverlay->TextBlock_DeathScore)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), InScore);
		CharacterOverlay->TextBlock_DeathScore->SetText(FText::FromString(ScoreText));
	}
}

void AEHUD::SetAMMOHUD(int32 AMMO)
{
	if (CharacterOverlay && CharacterOverlay->TextBlock_AMMO)
	{
		FString AMMOText = FString::Printf(TEXT("%d"), AMMO);
		CharacterOverlay->TextBlock_AMMO->SetText(FText::FromString(AMMOText));
	}
}

void AEHUD::SetCarriedAMMOHUD(int32 AMMO)
{
	if (CharacterOverlay && CharacterOverlay->TextBlock_CarriedAMMO)
	{
		FString AMMOText = FString::Printf(TEXT("%d"), AMMO);
		CharacterOverlay->TextBlock_CarriedAMMO->SetText(FText::FromString(AMMOText));
	}
}

void AEHUD::SetMatchCountdownHUD(float MatchCountdown)
{
	if (CharacterOverlay && CharacterOverlay->TextBlock_MatchCountDown)
	{
		if (MatchCountdown <= 0.f)
		{
			CharacterOverlay->TextBlock_MatchCountDown->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(MatchCountdown / 60.f);
		int32 Seconds = MatchCountdown - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		CharacterOverlay->TextBlock_MatchCountDown->SetText(FText::FromString(CountdownText));
	}
}

void AEHUD::SetAnnouncementHUD(float Countdown)
{
	if (Announcement && Announcement->TextBlock_WarmupTime)
	{
		if (Countdown <= 0.0f)
		{
			Announcement->TextBlock_WarmupTime->SetText(FText());
			return;
		}

		int32 Minutes = FMath::FloorToInt(Countdown / 60.f);
		int32 Seconds = Countdown - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d"), Seconds);
		Announcement->TextBlock_WarmupTime->SetText(FText::FromString(CountdownText));
	}
}

void AEHUD::AddAnnouncement()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && AnnouncementClass)
	{
		Announcement = CreateWidget<UEAnnouncementWidget>(PlayerController, AnnouncementClass);
		Announcement->AddToViewport();
	}
}

void AEHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f)  + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		FLinearColor::White
	);
}



void AEHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UECharacterOverlayWidget>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

