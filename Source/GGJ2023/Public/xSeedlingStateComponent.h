// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	void SetOwningPlayer(AxCharacter* NewOwner);

	UPROPERTY(BlueprintAssignable, Category = "Seedling")
	FOnOwningPlayerChanged OnOwningPlayerChanged;

protected:

	UFUNCTION()
	void OnRep_OwningPlayer(AxCharacter* PreviousOwner);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_OwningPlayer", Category = "Seedling")
	AxCharacter* OwningPlayer;


		
};
