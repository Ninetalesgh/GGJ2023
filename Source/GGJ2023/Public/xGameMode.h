// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "xGameMode.generated.h"

class AxCharacter;

UCLASS()
class GGJ2023_API AxGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AxGameMode();
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int PlayerCount;
};	

