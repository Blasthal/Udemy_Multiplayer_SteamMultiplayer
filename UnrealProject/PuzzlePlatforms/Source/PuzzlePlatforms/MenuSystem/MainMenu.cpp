// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "ScrollBox.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"

#include "ServerRow.h"
#include "TextBlock.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	// サーバー情報1行分
	const ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	check(ServerRowBPClass.Class);
	if (ServerRowBPClass.Class)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found class %s, in Constructor"), *ServerRowBPClass.Class->GetName());

		ServerRowClass = ServerRowBPClass.Class;
	}
}


void UMainMenu::SetServerList(TArray<FServerData> ServerDatas)
{
	UWorld* World = GetWorld();
	check(World);
	if (!World)
	{
		return;
	}

	check(ServerList);
	if (!ServerList)
	{
		return;
	}


	ServerList->ClearChildren();


	// TEST
	{
		FServerData ServerDataTest;
		ServerDataTest.Name = TEXT("Test Server Alpha");
		ServerDatas.Add(ServerDataTest);
	}
	{
		FServerData ServerDataTest;
		ServerDataTest.Name = TEXT("Test Server Bravo");
		ServerDatas.Add(ServerDataTest);
	}
	{
		FServerData ServerDataTest;
		ServerDataTest.Name = TEXT("Test Server Charlie");
		ServerDatas.Add(ServerDataTest);
	}


	uint32 i = 0;
	for (const FServerData& ServerData : ServerDatas)
	{
		UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
		check(ServerRow);
		if (!ServerRow)
		{
			return;
		}

		ServerRow->ServerName->SetText(FText::FromString(ServerData.Name));

		FString ServerConnectionFraction = FString::Format(_TEXT("{0}/{1}"), { ServerData.CurrentPlayers, ServerData.MaxPlayers });
		ServerRow->ConnectionFraction->SetText(FText::FromString(ServerConnectionFraction));

		ServerRow->HostUserName->SetText(FText::FromString(ServerData.HostUserName));

		ServerRow->Setup(this, i);
		++i;

		ServerList->AddChild(ServerRow);
	}
}


void UMainMenu::SetSelectedIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}


bool UMainMenu::Initialize()
{
	bool bIsSuccess = Super::Initialize();
	if (!bIsSuccess)
	{
		return false;
	}


	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	}

	if (JoinCancelButton)
	{
		JoinCancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	}

	if (JoinOkButton)
	{
		JoinOkButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	}

	if (ServerList)
	{
		// TODO
	}


	return true;
}


void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("HostServer - %s"), *HostButton->GetName());

	check(MenuInterface);
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}


void UMainMenu::JoinServer()
{
	check(MenuInterface);
	if (!MenuInterface)
	{
		return;
	}

	check(ServerList);
	if (!ServerList)
	{
		return;
	}

	
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d."), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set."));
	}


	MenuInterface->Join(SelectedIndex.GetValue());
}


void UMainMenu::OpenMainMenu()
{
	check(MenuSwitcher);
	if (!MenuSwitcher)
	{
		return;
	}

	check(MainMenu);
	if (!MainMenu)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenJoinMenu()
{
	check(MenuSwitcher);
	if (!MenuSwitcher)
	{
		return;
	}

	check(JoinMenu);
	if (!JoinMenu)
	{
		return;
	}

	MenuSwitcher->SetActiveWidget(JoinMenu);


	check(MenuInterface);
	if (!MenuInterface)
	{
		return;
	}

	ServerList->ClearChildren();
	MenuInterface->RefreshServerList();
}


void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UWidget* Widget = ServerList->GetChildAt(i);
		UServerRow* ServerRow = CastChecked<UServerRow>(Widget);
		if (!ServerRow) { continue; }

		if (!SelectedIndex.IsSet())
		{
			ServerRow->Selected = false;
			continue;
		}

		ServerRow->Selected = (SelectedIndex.GetValue() == i);
	}
}

