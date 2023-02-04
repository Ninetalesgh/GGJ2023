// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "xPlayerState.h"
#include "xHexGridTile.generated.h"

class UxFactionComponent;

UCLASS()
class GGJ2023_API AxHexGridTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AxHexGridTile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UxFactionComponent* FactionComp;

	virtual void BeginPlay() override;


};
