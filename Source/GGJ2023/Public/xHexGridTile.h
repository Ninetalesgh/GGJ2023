// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "xPlayerState.h"
#include "xHexGridTile.generated.h"

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

	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable, Category = "HexTile State")
		FOnFactionChanged OnFactionChanged;

	UFUNCTION(BlueprintCallable, Category = "HexTile State")
		void SetFaction(EFaction NewFaction);

	UFUNCTION(BlueprintCallable, Category = "HexTile State")
		EFaction GetFaction();

	UFUNCTION(BlueprintCallable, Category = "HexTile State")
		EFactionVariation GetFactionVariation();

protected:
	UPROPERTY(ReplicatedUsing = "OnRep_FactionChange")
		FFactionRepData RepData;

	UFUNCTION()
		void OnRep_FactionChange(FFactionRepData OldRepData);
};
