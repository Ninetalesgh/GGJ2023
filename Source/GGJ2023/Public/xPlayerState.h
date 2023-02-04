// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "xPlayerState.generated.h"

class AxAICharacter;

UCLASS()
class GGJ2023_API AxPlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	AxPlayerState();

	UFUNCTION(BlueprintPure, Category = "Credits")
	static AxPlayerState* GetPlayerStateFromActor(AActor* FromActor);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<AxAICharacter*> PlantedSeedlings;

protected:

	virtual void BeginPlay() override;
};
