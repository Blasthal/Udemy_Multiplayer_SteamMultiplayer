// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MenuWidget.h"

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
	virtual void Host() override;
	
	UFUNCTION(Exec)
	virtual void Join(const FString& Address) override;

	UFUNCTION(Exec)
	virtual void LoadMainMenu() override;


private:
	TSubclassOf<UUserWidget> MenuClass = nullptr;
	UMainMenu* MainMenu = nullptr;

	TSubclassOf<UUserWidget> InGameMenuClass = nullptr;
	UInGameMenu* InGameMenu = nullptr;
};
