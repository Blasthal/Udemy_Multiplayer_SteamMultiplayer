// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "PlatformTrigger.h"


namespace
{
	const static FName SessionName = TEXT("My Session Game");
}


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

		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Founded session interface"));

			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);

			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				//SessionSearch->bIsLanQuery = true;

				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

				UE_LOG(LogTemp, Warning, TEXT("FindSessions Start"));
			}
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
	if (SessionInterface.IsValid())
	{
		// 既にセッションがある場合は、セッションを破棄する
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SessionName);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(SessionName);
		}
		else
		{
			CreateSession();
		}
	}
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


void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	ensure(bSuccess);
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session. SessionName=%s"), *SessionName.ToString());
		return;
	}


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


	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete() SessionName=%s"), *SessionName.ToString());
	Engine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, *FString::Format(TEXT("SessionName={0}"), { *SessionName.ToString() } ));

	//World->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap"));
	World->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen"));
}


void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete() SessionName=%s"), *SessionName.ToString());

	if (bSuccess)
	{
		CreateSession();
	}
}


void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete()"));

	if (bSuccess)
	{
		if (SessionSearch.IsValid())
		{
			for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("Find Session Id Str: %s"), *SearchResult.GetSessionIdStr());
			}
		}
	}
}


void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, SessionName, SessionSettings);
	}
}
