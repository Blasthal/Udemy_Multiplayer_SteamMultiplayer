// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"

#include "PlatformTrigger.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance Constructed"));

	// メインメニュー
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	check(MenuBPClass.Class);
	if (!MenuBPClass.Class)
	{
		return;
	};

	UE_LOG(LogTemp, Warning, TEXT("Found class %s, in Constructor"), *MenuBPClass.Class->GetName());

	MenuClass = MenuBPClass.Class;

	// インゲームメニュー
	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	check(InGameMenuBPClass.Class);
	if (!InGameMenuBPClass.Class)
	{
		return;
	};

	UE_LOG(LogTemp, Warning, TEXT("Found class %s, in Constructor"), *InGameMenuBPClass.Class->GetName());

	InGameMenuClass = InGameMenuBPClass.Class;
}


void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("<UPuzzlePlatformsGameInstance Init>"));

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Founded subsystem %s"), *Subsystem->GetSubsystemName().ToString());

		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Founded session interface"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Founded subsystem"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Founded class %s"), *MenuClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Founded class %s"), *InGameMenuClass->GetName());
}


void UPuzzlePlatformsGameInstance::LoadMenu()
{
	check(MenuClass);
	if (!MenuClass)
	{
		return;
	}

	MainMenu = CreateWidget<UMainMenu>(this, MenuClass);
	check(MainMenu);
	if (!MainMenu)
	{
		return;
	}

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}


void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	check(InGameMenuClass);
	if (!InGameMenuClass)
	{
		return;
	}

	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
	check(InGameMenu);
	if (!InGameMenu)
	{
		return;
	}

	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}


void UPuzzlePlatformsGameInstance::Host()
{
	check(MainMenu);
	if (MainMenu)
	{
		MainMenu->TearDown();
	}


	UEngine* Engine = GetEngine();
	check(Engine);
	if (!Engine)
	{
		return;
	}


	UWorld* World = GetWorld();
	check(World);
	if (!World)
	{
		return;
	}


	UE_LOG(LogTemp, Warning, TEXT("Hosting"));
	Engine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("Hosting"));

	//World->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap"));
	World->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen"));
}


void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	check(MainMenu);
	if (MainMenu)
	{
		MainMenu->TearDown();
	}


	UEngine* Engine = GetEngine();
	check(Engine);
	if (!Engine)
	{
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	check(PlayerController);
	if (!PlayerController)
	{
		return;
	}


	UE_LOG(LogTemp, Warning, TEXT("Joining %s"), *Address);
	Engine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}


void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	UEngine* Engine = GetEngine();
	check(Engine);
	if (!Engine)
	{
		return;
	}

	UWorld* World = GetWorld();
	check(World);
	if (!World)
	{
		return;
	}
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	check(PlayerController);
	if (!PlayerController)
	{
		return;
	}


	UE_LOG(LogTemp, Warning, TEXT("LoadMainMenu"));
	Engine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, TEXT("LoadMainMenu"));

	PlayerController->ClientTravel(TEXT("/Game/MenuSystem/MainMenu"), ETravelType::TRAVEL_Absolute);
}
