// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "Button.h"
#include "MainMenu.h"


bool UServerRow::Initialize()
{
	Super::Initialize();


	// ƒ{ƒ^ƒ“
	if (RowButton)
	{
		RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
	}


	return true;
}


void UServerRow::Setup(UMainMenu* Parent, uint32 Index)
{
	this->MainMenu = Parent;
	this->Index = Index;
}


void UServerRow::OnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnClicked() SelectedIndex=%d"), Index);

	check(MainMenu);
	if (MainMenu)
	{
		MainMenu->SetSelectedIndex(Index);
	}
}
