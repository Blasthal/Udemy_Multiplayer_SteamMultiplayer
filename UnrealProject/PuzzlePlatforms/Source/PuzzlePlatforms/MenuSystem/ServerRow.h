// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;


	// �T�[�o�[��
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName = nullptr;

	// �Q���l�� / �ő�l��
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConnectionFraction = nullptr;

	// �z�X�g��
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUserName = nullptr;


	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(class UMainMenu* Parent, uint32 Index);

private:
	UFUNCTION()
	void OnClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton = nullptr;

	UPROPERTY()
	class UMainMenu* MainMenu = nullptr;

	uint32 Index = 0;
};
