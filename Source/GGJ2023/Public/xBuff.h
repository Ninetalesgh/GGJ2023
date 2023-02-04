// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "xAction.h"
#include "Engine/EngineTypes.h"
#include "xBuff.generated.h"

/**
 * 
 */
UCLASS()
class GGJ2023_API UxBuff : public UxAction
{
	GENERATED_BODY()
public:

	UxBuff();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UAnimMontage* PeriodicAnimation;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
