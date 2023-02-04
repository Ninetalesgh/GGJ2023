// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "xAction.h"
#include "xAction_Plant.generated.h"


class AxCharacter;
class UParticleSystem;


UCLASS()
class GGJ2023_API UxAction_Plant : public UxAction
{
	GENERATED_BODY()

public:
	void ServerOnlyActionPart_Implementation(AActor* InstigatorActor) override;
	UxAction_Plant();
protected:

	UPROPERTY(EditAnywhere, Category = "Action")
	UParticleSystem* ActionEffect;

	UFUNCTION()
	void ActionDelay_Elapsed(AxCharacter* InstigatorCharacter);
};
