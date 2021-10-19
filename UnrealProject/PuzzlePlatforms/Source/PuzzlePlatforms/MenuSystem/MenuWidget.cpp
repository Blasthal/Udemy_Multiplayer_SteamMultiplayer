// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"


void UMenuWidget::Setup()
{
	bIsFocusable = true;
	AddToViewport();


	UWorld* World = GetWorld();
	check(World);
	if (!World)
	{
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	ensure(PlayerController);
	if (!PlayerController)
	{
		return;
	}


	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}


void UMenuWidget::TearDown()
{
	bIsFocusable = false;
	RemoveFromViewport();


	UWorld* World = GetWorld();
	check(World);
	if (!World)
	{
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	check(PlayerController);
	if (!PlayerController)
	{
		return;
	}

	FInputModeGameOnly InputModeData;
	InputModeData.SetConsumeCaptureMouseDown(true);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}


void UMenuWidget::SetMenuInterface(IMenuInterface* inMenuInterface)
{
	this->MenuInterface = inMenuInterface;
}

