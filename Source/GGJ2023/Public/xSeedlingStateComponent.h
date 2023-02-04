// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "xPlayerState.h"
#include "xSeedlingStateComponent.generated.h"

class AxAICharacter;
class AxCharacter;

UENUM(BlueprintType)
enum ESeedlingState
{
	SeedlingState_Planted,
	SeedlingState_Uprooted,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSeedlingStateChanged, AxAICharacter*, Seedling, ESeedlingState, NewSeedlingState, ESeedlingState, OldSeedlingState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnOwningPlayerChanged, AxAICharacter*, Seedling, AxCharacter*, NewOwningPlayer, AxCharacter*, OldOwningPlayer);

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
	FOnSeedlingStateChanged OnSeedlingStateChanged;

protected:

	UFUNCTION()
	void OnRep_OwningPlayerChange(AxCharacter* PreviousOwner);

	UFUNCTION()
	void OnRep_SeedlingStateChange(ESeedlingState PreviousState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_SeedlingStateChange", Category = "Seedling Properties")
	TEnumAsByte<ESeedlingState> State;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_OwningPlayerChange", Category = "Seedling Properties")
	AxCharacter* OwningPlayer;		
};
