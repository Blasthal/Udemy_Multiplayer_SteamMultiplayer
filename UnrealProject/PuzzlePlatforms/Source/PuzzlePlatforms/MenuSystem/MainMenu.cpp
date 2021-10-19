// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


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
	UE_LOG(LogTemp, Warning, TEXT("JoinServer(%s) - %s"), *IPAddressField->GetText().ToString(), *HostButton->GetName());

	check(MenuInterface);
	if (MenuInterface)
	{
		MenuInterface->Join(IPAddressField->GetText().ToString());
	}
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
}

