// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"

#include "MainMenu.generated.h"


USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerDatas);

	void SetSelectedIndex(uint32 Index);

private:
	virtual bool Initialize() override;
	
	
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenJoinMenu();


	void UpdateChildren();


	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinCancelButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinOkButton = nullptr;


	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList = nullptr;

	TSubclassOf<class UUserWidget> ServerRowClass = nullptr;

	TOptional<uint32> SelectedIndex;
};
