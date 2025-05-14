// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};


UCLASS()
class EMULTIPLAYERSESSIONS_API UEMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString LobbyPath = FString(TEXT("/Game/Levels/Lobby")));

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

protected :
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful, FName ServerKey);
	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private :
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void ServerListResetButtonClicked();


private:
	void MenuTearDown();
	void UpdateChildren();

public :
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> ServerRowClass;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Host;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Join;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_CancelJoinMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_ConfirmJoinMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidget> MainMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidget> JoinMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidget> HostMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> EditableTextBox_ServerHostName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_CancelHostMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_ConfirmHostMenu;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPanelWidget> Panel_ServerList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_ServerListReset;

private :
	class UEMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	int32 NumPublicConnections{ 4 };
	FString PathToLobby{ TEXT("") };

	TOptional<uint32> SelectedIndex;


};
