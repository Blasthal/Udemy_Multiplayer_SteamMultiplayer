// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance
	: public UGameInstance
	, public IMenuInterface
{
	GENERATED_BODY()
	

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();


	UFUNCTION(Exec)
	virtual void Host(FString ServerName) override;
	
	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;

	void StartSession();

	UFUNCTION(Exec)
	virtual void LoadMainMenu() override;

	virtual void RefreshServerList() override;


private:
	TSubclassOf<UUserWidget> MenuClass = nullptr;
	UMainMenu* MainMenu = nullptr;

	TSubclassOf<UUserWidget> InGameMenuClass = nullptr;
	UInGameMenu* InGameMenu = nullptr;

	IOnlineSessionPtr SessionInterface = nullptr;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	void OnFindSessionsComplete(bool bSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	FString DesiredServerName;
	void CreateSession();
};
