// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "xPlayerState.generated.h"

class AxAICharacter;

UENUM(BlueprintType)
enum EFaction
{
	Faction_Unassigned,
	Faction_Leafy,
	Faction_Succulent,
	Faction_Mushroom,
	Faction_Flower
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFactionChanged, ACharacter*, Character, EFaction, OldFaction, EFaction, NewFaction);


UCLASS()
class GGJ2023_API AxPlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	AxPlayerState();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<AxAICharacter*> PlantedSeedlings;

	UPROPERTY(BlueprintAssignable, Category = "Player State")
	FOnFactionChanged OnFactionChanged;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	void SetFaction(EFaction NewFaction);

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing="OnRep_FactionChange", Category = "Player State")
	TEnumAsByte<EFaction> Faction;

	UFUNCTION()
	void OnRep_FactionChange(EFaction PreviousFaction);
};
