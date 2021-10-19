// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuSystem/MenuWidget.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()


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
	class UEditableTextBox* IPAddressField = nullptr;
};
