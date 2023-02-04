// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "xGameMode.generated.h"

class AxCharacter;
class AxAICharacter;

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



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int PlayerCount;



};	

