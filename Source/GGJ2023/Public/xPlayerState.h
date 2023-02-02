// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "xPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GGJ2023_API AxPlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	AxPlayerState();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int PlayerIndex;
};
