// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()


public:
	AMovingPlatform();


protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaSeconds) override;


private:
	// ˆÚ“®‘¬“x
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 100.0f;

	// –Ú•WˆÊ’u
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation = FVector::ZeroVector;


public:
	void AddActiveTrigger();
	void RemoveActiveTrigger();


private:
	UPROPERTY()
	FVector GlobalTargetLocation = FVector::ZeroVector;

	UPROPERTY()
	FVector GlobalStartLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	int32 ActiveTriggers = 1;
};
