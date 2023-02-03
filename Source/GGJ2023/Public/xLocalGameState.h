// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "xLocalGameState.generated.h"

class APlayerController;

UCLASS()
class GGJ2023_API AxLocalGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	FDelegateHandle InputDeviceChangedDelegate;
};
