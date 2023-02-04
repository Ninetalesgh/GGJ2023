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

UENUM(BlueprintType)
enum EFactionVariation
{
	FactionVariation_0,
	FactionVariation_1,
	FactionVariation_2,
	FactionVariation_3,
	FactionVariation_4,
};

USTRUCT()
struct FFactionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TEnumAsByte<EFactionVariation> Variation;

	UPROPERTY()
	TEnumAsByte<EFaction> Faction;

	bool operator ==(FFactionRepData const& Other) { return Variation == Other.Variation && Faction == Other.Faction; }
	bool operator !=(FFactionRepData const& Other) { return !(*this == Other); }
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnFactionChanged, AActor*, Instigator, EFaction, NewFaction, EFactionVariation, NewVariation, EFaction, OldFaction, EFactionVariation, OldVariation);

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

	UPROPERTY(BlueprintAssignable, Category = "Player State")
	FOnFactionChanged OnFactionChanged;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	void SetFaction(EFaction NewFaction);

	UFUNCTION(BlueprintCallable, Category = "Player State")
	EFaction GetFaction() const;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	EFactionVariation GetFactionVariation() const;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	void SetFactionVariation(EFactionVariation NewVariation);

protected:
	UPROPERTY(ReplicatedUsing="OnRep_FactionChange")
	FFactionRepData RepData;

	UFUNCTION()
	void OnRep_FactionChange(FFactionRepData OldRepData);
};
