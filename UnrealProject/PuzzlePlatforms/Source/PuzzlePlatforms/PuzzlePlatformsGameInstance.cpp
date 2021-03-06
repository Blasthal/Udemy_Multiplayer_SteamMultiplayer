// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"


namespace
{
	static const FName SESSION_NAME = TEXT("GameSession"); // OSSがNULLの場合はセッション名が強制的に"GameSession"で処理される箇所があるため、揃える
	static const FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");
	static const uint32 SESSION_MAX_PLAYERS = 5;
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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Founded subsystem"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Founded class %s"), *MenuClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Founded class %s"), *InGameMenuClass->GetName());


	// 切断時のエラーコールバック登録
	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UPuzzlePlatformsGameInstance::OnNetworkFailure);
	}
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


void UPuzzlePlatformsGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid())
	{
		// 既にセッションがある場合は、セッションを破棄する
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}


void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	check(SessionInterface);
	check(SessionSearch.IsValid());

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}


void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
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


void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		// trueじゃない場合はLANとそれ以外の両方を見るようになるらしい
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

		UE_LOG(LogTemp, Warning, TEXT("FindSessions Start"));
	}
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

	World->ServerTravel(TEXT("/Game/PuzzlePlatforms/Maps/Lobby?listen"));
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

	if (!bSuccess)
	{
		return;
	}

	if (!SessionSearch.IsValid())
	{
		return;
	}


	TArray<FServerData> ServerDatas;
	for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Session Id Str: %s"), *SearchResult.GetSessionIdStr());

		FServerData ServerData;
		ServerData.CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
		ServerData.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
		ServerData.HostUserName = SearchResult.Session.OwningUserName;

		FString ServerName;
		if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
		{
			ServerData.Name = ServerName;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find name."));
		}

		ServerDatas.Add(ServerData);
	}

	MainMenu->SetServerList(ServerDatas);
}


void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
	{
		check(nullptr);
		return;
	}

	check(MainMenu);
	if (MainMenu)
	{
		MainMenu->TearDown();
	}


	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, Address))
	{
		return;
	}


	APlayerController* PlayerController = GetFirstLocalPlayerController();
	check(PlayerController);
	if (!PlayerController)
	{
		return;
	}


	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete() Address=%s"), *Address);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Green, FString::Format(TEXT("JoinSession {0}"), { *Address }));
	}


	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}


void UPuzzlePlatformsGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ErrorString);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Yellow, ErrorString);
	}

	LoadMainMenu();
}


void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		if (Subsystem &&
			Subsystem->GetSubsystemName() == TEXT("NULL")
			)
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}

		SessionSettings.NumPublicConnections = SESSION_MAX_PLAYERS;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		//SessionSettings.bUseLobbiesIfAvailable = true; // UE4.27以降で必要らしい
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}
