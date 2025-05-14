// Fill out your copyright notice in the Description page of Project Settings.


#include "EMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "EMultiplayerSessionsSubsystem.h"
#include "EServerRow.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

bool UEMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Button_Host)
	{
		Button_Host->OnClicked.AddDynamic(this, &ThisClass::OpenHostMenu);
	}

	if (Button_CancelHostMenu)
	{
		Button_CancelHostMenu->OnClicked.AddDynamic(this, &ThisClass::OpenMainMenu);
	}

	if (Button_ConfirmHostMenu)
	{
		Button_ConfirmHostMenu->OnClicked.AddDynamic(this, &ThisClass::HostServer);
	}

	if (Button_Join)
	{
		Button_Join->OnClicked.AddDynamic(this, &ThisClass::OpenJoinMenu);
	}

	if (Button_CancelJoinMenu)
	{
		Button_CancelJoinMenu->OnClicked.AddDynamic(this, &ThisClass::OpenMainMenu);
	}

	if (Button_ConfirmJoinMenu)
	{
		Button_ConfirmJoinMenu->OnClicked.AddDynamic(this, &ThisClass::JoinServer);
	}

	if (Button_ServerListReset)
	{
		Button_ServerListReset->OnClicked.AddDynamic(this, &ThisClass::ServerListResetButtonClicked);
	}

	return true;
}

void UEMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

void UEMenu::MenuSetup(int32 NumberOfPublicConnections, FString LobbyPath)
{
	{
		PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
		NumPublicConnections = NumberOfPublicConnections;
	}

	{
		AddToViewport();
		SetVisibility(ESlateVisibility::Visible);
		SetIsFocusable(true);

		if (UWorld* World = GetWorld())
		{
			APlayerController* PlayerController = World->GetFirstPlayerController();
			if (PlayerController)
			{
				FInputModeUIOnly InputModeData;
				InputModeData.SetWidgetToFocus(TakeWidget());
				InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(InputModeData);
				PlayerController->SetShowMouseCursor(true);
			}
		}
	}

	{
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UEMultiplayerSessionsSubsystem>();
		}

		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
			MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
			MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
			MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
		}
	}
}

void UEMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	Panel_ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UEServerRow* Row = CreateWidget<UEServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->TextBlock_ServerName->SetText(FText::FromString(ServerData.Name));
		Row->TextBlock_HostUser->SetText(FText::FromString(ServerData.HostUsername));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->TextBlock_ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->Setup(this, i);
		++i;

		Panel_ServerList->AddChild(Row);
	}
}

void UEMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}


void UEMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Failed to create session!")));
		}
		Button_Host->SetIsEnabled(true);
	}
}

void UEMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful, FName ServerKey)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}


	TArray<FServerData> ServerNames;

	for (const FOnlineSessionSearchResult & Result : SessionResults)
	{
		FServerData Data;
		Data.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - Result.Session.NumOpenPublicConnections;
		Data.HostUsername = Result.Session.OwningUserName;

		FString ServerName;
		if (Result.Session.SessionSettings.Get(ServerKey, ServerName))
		{
			Data.Name = ServerName;
		}
		else
		{
			Data.Name = "Could not find name.";
		}
		ServerNames.Add(Data);
	}

	SetServerList(ServerNames);

	Button_Join->SetIsEnabled(true);
}

void UEMenu::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(SessionName, Address);

			if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UEMenu::OnDestroySession(bool bWasSuccessful)
{
}

void UEMenu::OnStartSession(bool bWasSuccessful)
{
}

void UEMenu::HostServer()
{
	Button_Host->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		FString ServerName = EditableTextBox_ServerHostName->Text.ToString();
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, ServerName);
	}
}

void UEMenu::JoinServer()
{
	Button_Join->SetIsEnabled(false);

	if (SelectedIndex.IsSet())
	{
		MultiplayerSessionsSubsystem->JoinSession(SelectedIndex.GetValue());
	}
}

void UEMenu::OpenHostMenu()
{
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UEMenu::OpenJoinMenu()
{
	if (!MenuSwitcher) return;
	if (!JoinMenu) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UEMenu::OpenMainMenu()
{
	if (!MenuSwitcher) return;
	if (!JoinMenu) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UEMenu::ServerListResetButtonClicked()
{
	if (!JoinMenu) return;

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UEMenu::MenuTearDown()
{
	RemoveFromParent();
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}


void UEMenu::UpdateChildren()
{
	for (int32 i = 0; i < Panel_ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UEServerRow>(Panel_ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);

			Row->SetRowColor();
		}
	}
}
