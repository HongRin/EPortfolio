// Fill out your copyright notice in the Description page of Project Settings.


#include "EMultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");
const static FName CURRENT_PLAYER_KEY = TEXT("CurrentPlayer");


UEMultiplayerSessionsSubsystem::UEMultiplayerSessionsSubsystem() :
	CreateSessionCompleteDelegate ( FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete )),
	FindSessionsCompleteDelegate  (  FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete  )),
	JoinSessionCompleteDelegate   (   FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete   )),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate  (  FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete  ))
{

}

void UEMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString ServerName)
{
	DesiredServerName = ServerName;

	if (!IsValidSessionInterface())
	{
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession != nullptr)
	{
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		DesiredServerName = ServerName;

		SessionInterface->DestroySession(SESSION_NAME);
	}

	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	{
		LastSessionSettings->bIsLANMatch            = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
		LastSessionSettings->NumPublicConnections   = NumPublicConnections;
		LastSessionSettings->bAllowJoinInProgress   = true;
		LastSessionSettings->bAllowJoinViaPresence  = true;
		LastSessionSettings->bShouldAdvertise       = true;
		LastSessionSettings->bUsesPresence          = true;
		LastSessionSettings->bUseLobbiesIfAvailable = true;
		LastSessionSettings->BuildUniqueId          = 1;
		LastSessionSettings->Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		LastSessionSettings->Set(CURRENT_PLAYER_KEY, 1, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	}

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), SESSION_NAME, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}

void UEMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!IsValidSessionInterface())
	{
		return;
	}

	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false, SERVER_NAME_SETTINGS_KEY);
	}
}

void UEMultiplayerSessionsSubsystem::JoinSession(uint32 Index)
{
	if (!IsValidSessionInterface())
	{
		MultiplayerOnJoinSessionComplete.Broadcast(SESSION_NAME, EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), SESSION_NAME, LastSessionSearch->SearchResults[Index]))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		MultiplayerOnJoinSessionComplete.Broadcast(SESSION_NAME, EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UEMultiplayerSessionsSubsystem::DestroySession()
{
	if (!IsValidSessionInterface())
	{
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(SESSION_NAME))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void UEMultiplayerSessionsSubsystem::StartSession()
{
}

bool UEMultiplayerSessionsSubsystem::IsValidSessionInterface()
{
	if (!SessionInterface)
	{
		if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
		{
			SessionInterface = Subsystem->GetSessionInterface();
		}
	}
	return SessionInterface.IsValid();
}

void UEMultiplayerSessionsSubsystem::SetPlayerCount(int32 PlayerCount)
{
	if (!IsValidSessionInterface())
	{
		return;
	}

	FNamedOnlineSession* Session = SessionInterface->GetNamedSession(SESSION_NAME);


	if (Session)
	{
		Session->SessionSettings.Set(CURRENT_PLAYER_KEY, PlayerCount, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->UpdateSession(SESSION_NAME, Session->SessionSettings);
	}
}

int32 UEMultiplayerSessionsSubsystem::GetPlayerCount()
{
	if (!IsValidSessionInterface())
	{
		return 0;
	}

	FNamedOnlineSession* Session = SessionInterface->GetNamedSession(SESSION_NAME);


	if (Session)
	{
		return Session->NumOpenPublicConnections;
	}

	return 0;
}

void UEMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UEMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false, SERVER_NAME_SETTINGS_KEY);
		return;
	}

	MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful, SERVER_NAME_SETTINGS_KEY);
}

void UEMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	MultiplayerOnJoinSessionComplete.Broadcast(SessionName, Result);
}

void UEMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	if (bWasSuccessful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
		CreateSession(LastNumPublicConnections, DesiredServerName);
	}
	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UEMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}
