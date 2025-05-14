// Copyright EPortfolio


#include "HUD/ESystemMenuWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "EMultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UESystemMenuWidget::MenuSetup()
{
	Super::MenuSetup();

	SetButtonTextColor(TextBlock_Return, FLinearColor::White);
	SetButtonTextColor(TextBlock_LeaveGame, FLinearColor::White);
	SetButtonTextColor(TextBlock_Exittodesktop, FLinearColor::White);

	bShowSystemMenu = true;
}

void UESystemMenuWidget::MenuTearDown()
{
	Super::MenuTearDown();

	bShowSystemMenu = false;
}

bool UESystemMenuWidget::Initialize()
{
    if (!Super::Initialize())
    {
        return false;
    }

	if (Button_LeaveGame)
	{
		Button_Return->OnClicked.AddDynamic(this, &UESystemMenuWidget::ReturnButtonClicked);
		Button_Return->OnHovered.AddDynamic(this, &UESystemMenuWidget::ReturnButtonHovered);
		Button_Return->OnUnhovered.AddDynamic(this, &UESystemMenuWidget::ReturnButtonUnhovered);

		Button_LeaveGame->OnClicked.AddDynamic(this, &UESystemMenuWidget::LeaveGameButtonClicked);
		Button_LeaveGame->OnHovered.AddDynamic(this, &UESystemMenuWidget::LeaveGameButtonHovered);
		Button_LeaveGame->OnUnhovered.AddDynamic(this, &UESystemMenuWidget::LeaveGameButtonUnhovered);

		Button_ExitToDesktop->OnClicked.AddDynamic(this, &UESystemMenuWidget::ExitToDesktopButtonClicked);
		Button_ExitToDesktop->OnHovered.AddDynamic(this, &UESystemMenuWidget::ExitToDesktopButtonHovered);
		Button_ExitToDesktop->OnUnhovered.AddDynamic(this, &UESystemMenuWidget::ExitToDesktopButtonUnhovered);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UEMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UESystemMenuWidget::OnDestroySession);
		}
	}

    return  true;
}

void UESystemMenuWidget::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		Button_LeaveGame->SetIsEnabled(true);
		return;
	}

	if (UWorld* World = GetWorld())
	{
		if (AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>())
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UESystemMenuWidget::ReturnButtonClicked()
{
	MenuTearDown();
	SetButtonTextColor(TextBlock_Return, ClickedTextColor);
}

void UESystemMenuWidget::ReturnButtonHovered()
{
	SetButtonTextColor(TextBlock_Return, HoveredTextColor);
}

void UESystemMenuWidget::ReturnButtonUnhovered()
{
	SetButtonTextColor(TextBlock_Return, FLinearColor::White);
}

void UESystemMenuWidget::LeaveGameButtonClicked()
{
	Button_LeaveGame->SetIsEnabled(false);
	SetButtonTextColor(TextBlock_LeaveGame, ClickedTextColor);

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}

void UESystemMenuWidget::LeaveGameButtonHovered()
{
	SetButtonTextColor(TextBlock_LeaveGame, HoveredTextColor);
}

void UESystemMenuWidget::LeaveGameButtonUnhovered()
{
	SetButtonTextColor(TextBlock_LeaveGame, FLinearColor::White);
}

void UESystemMenuWidget::ExitToDesktopButtonClicked()
{
	if (UWorld* World = GetWorld())
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
	}

	SetButtonTextColor(TextBlock_Exittodesktop, ClickedTextColor);
}

void UESystemMenuWidget::ExitToDesktopButtonHovered()
{
	SetButtonTextColor(TextBlock_Exittodesktop, HoveredTextColor);
}

void UESystemMenuWidget::ExitToDesktopButtonUnhovered()
{
	SetButtonTextColor(TextBlock_Exittodesktop, FLinearColor::White);
}

void UESystemMenuWidget::SetButtonTextColor(UTextBlock* TextBlock, const FLinearColor & Color)
{
	TextBlock->SetColorAndOpacity(Color);
}
