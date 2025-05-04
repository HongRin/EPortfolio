// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EMultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class EMULTIPLAYERSESSIONS_API UEMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UEMultiplayerSessionsSubsystem();

public :
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();

	bool IsValidSessionInterface();

protected:
	void OnCreateSessionComplete (FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete  (bool bWasSuccessful);
	void OnJoinSessionComplete   (FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete  (FName SessionName, bool bWasSuccessful);

public :
	FMultiplayerOnCreateSessionComplete  MultiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionsComplete   MultiplayerOnFindSessionsComplete;
	FMultiplayerOnJoinSessionComplete    MultiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete   MultiplayerOnStartSessionComplete;

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch>   LastSessionSearch;

private:
	FOnCreateSessionCompleteDelegate  CreateSessionCompleteDelegate;
	FDelegateHandle                   CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate   FindSessionsCompleteDelegate;
	FDelegateHandle                   FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate    JoinSessionCompleteDelegate;
	FDelegateHandle                   JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle                   DestroySessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate   StartSessionCompleteDelegate;
	FDelegateHandle                   StartSessionCompleteDelegateHandle;

private:
	bool bCreateSessionOnDestroy{ false };
	int32 LastNumPublicConnections;
	FString LastMatchType;
};
