// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "xGameMode.generated.h"


class AxCharacter;
class AxAICharacter;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

UCLASS()
class GGJ2023_API AxGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AxGameMode();
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void GenerateHexGrid();

	void DrawSeedlingTriangle(AxCharacter* InstigatorPlayer, TArray<AxAICharacter*> Seedlings);
	void UprootSeedling(AxAICharacter* Seedling);

	UPROPERTY(EditAnywhere, Category = "GGJ2023")
	TSubclassOf<AActor> FollowerClass;
	
	UPROPERTY(EditAnywhere, Category = "GGJ2023")
	TSubclassOf<AActor> HexGridTileClass;

protected:
	FTimerHandle TimerHandle_SpawnSeedlings;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnSeedlingQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void SpawnSeedlingsTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);


};	

