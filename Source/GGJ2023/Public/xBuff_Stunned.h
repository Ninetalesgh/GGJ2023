// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "xBuff.h"
#include "xBuff_Stunned.generated.h"

/**
 * 
 */
UCLASS()
class GGJ2023_API UxBuff_Stunned : public UxBuff
{
	GENERATED_BODY()
public:
	UxBuff_Stunned();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;


protected:

	//UFUNCTION()
	//void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
