// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "xAction.generated.h"

class UxActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class GGJ2023_API UxAction : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(UxActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable)
	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual bool IsSupportedForNetworking() const override { return true; }
protected:

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	//TSoftObjectPtr<UTexture2D> Icon;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UxActionComponent* GetOwningComponent() const;

	//Action can only start if OwningActor has none of these Tags applied
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTagContainer GrantsTags;

	//Tags added to owning actor when activated, removed when action stops
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UPROPERTY(Replicated)
	UxActionComponent* ActionComp;

	UFUNCTION()
	void OnRep_RepData();

	UPROPERTY(Replicated)
	float TimeStarted;
};
