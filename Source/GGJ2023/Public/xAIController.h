// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "xAIController.generated.h"

/**
 * 
 */
UCLASS()
class GGJ2023_API AxAIController : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	virtual void BeginPlay() override;
};
