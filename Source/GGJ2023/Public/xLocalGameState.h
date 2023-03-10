// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "xLocalGameState.generated.h"

class APlayerController;
class UCameraComponent;
class USceneComponent;

UCLASS()
class GGJ2023_API AxLocalGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AxLocalGameState();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	FDelegateHandle InputDeviceChangedDelegate;	

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category="Game State")
	FVector MainCameraLocation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Game State")
	FVector MainCameraForward;
protected:
};
