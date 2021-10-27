// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "PuzzlePlatformsGameInstance.h"
#include "TimerManager.h"
#include "Engine/World.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("PostLogin NumberOfPlayers: %d"), NumberOfPlayers);

	constexpr uint32 MaxNumberOfPlayers = 3;
	if (MaxNumberOfPlayers <= NumberOfPlayers)
	{
		constexpr float WaitTime = 5.0f;
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, WaitTime);
	}
}


void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("Logout NumberOfPlayers: %d"), NumberOfPlayers);
}


void ALobbyGameMode::StartGame()
{
	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	check(GameInstance);
	if (!GameInstance)
	{
		return;
	}

	GameInstance->StartSession();


	UWorld* World = GetWorld();
	check(World);
	if (!World)
	{
		return;
	}

	bUseSeamlessTravel = true;
	World->ServerTravel(TEXT("/Game/PuzzlePlatforms/Maps/Game?listen"));
}
