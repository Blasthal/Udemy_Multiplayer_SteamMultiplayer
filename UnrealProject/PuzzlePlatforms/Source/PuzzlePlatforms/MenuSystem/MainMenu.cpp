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


void UMainMenu::SetServerList(TArray<FString> ServerNames)
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

	for (const FString& ServerName : ServerNames)
	{
		UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
		check(ServerRow);
		if (!ServerRow)
		{
			return;
		}

		ServerRow->ServerName->SetText(FText::FromString(ServerName));

		ServerList->AddChild(ServerRow);
	}
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


	//UE_LOG(LogTemp, Warning, TEXT("JoinServer(%s) - %s"), *IPAddressField->GetText().ToString(), *HostButton->GetName());

	//MenuInterface->Join(ServerList->selected->GetText().ToString());

	//UWorld* World = GetWorld();
	//check(World);
	//if (!World)
	//{
	//	return;
	//}

	//UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
	//check(ServerRow);
	//if (!ServerRow)
	//{
	//	return;
	//}

	//ServerRow->ServerName->SetText(FText::FromString(TEXT("Test")));

	//ServerList->AddChild(ServerRow);
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

