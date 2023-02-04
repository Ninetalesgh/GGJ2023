// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "xPlayerState.h"
#include "xSeedlingStateComponent.generated.h"

class AxAICharacter;
class AxCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnOwningPlayerChanged, AxAICharacter*, Seedling, AxCharacter*, PreviousOwningPlayer, AxCharacter*, NewOwningPlayer);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GGJ2023_API UxSeedlingStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UxSeedlingStateComponent();

	UFUNCTION(BlueprintCallable, Category = "Seedling State")
	void SetOwningPlayer(AxCharacter* NewOwner);

	UPROPERTY(BlueprintAssignable, Category = "Seedling State")
	FOnOwningPlayerChanged OnOwningPlayerChanged;

	UPROPERTY(BlueprintAssignable, Category = "Seedling State")
	FOnFactionChanged OnFactionChanged;

	UFUNCTION(BlueprintCallable, Category = "Seedling State")
	void SetFaction(EFaction NewFaction);

	UFUNCTION(BlueprintCallable, Category = "Seedling State")
	EFaction GetFaction() const;

	UFUNCTION(BlueprintCallable, Category = "Seedling State")
	EFactionVariation GetFactionVariation() const;

	UFUNCTION(BlueprintCallable, Category = "Seedling State")
	void SetFactionVariation(EFactionVariation NewVariation);

protected:

	UFUNCTION()
	void OnRep_OwningPlayerChange(AxCharacter* PreviousOwner);
	
	UPROPERTY(ReplicatedUsing = "OnRep_FactionChange")
	FFactionRepData RepData;

	UFUNCTION()
	void OnRep_FactionChange(FFactionRepData OldRepData);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_OwningPlayerChange", Category = "Seedling Properties")
	AxCharacter* OwningPlayer;		
};
