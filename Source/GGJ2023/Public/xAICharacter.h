// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "xAICharacter.generated.h"

class UxActionComponent;
class UxAttributeComponent;
class AxCharacter;
class UxSeedlingStateComponent;

//class UUserWidget;
//class UxWorldUserWidget;


UCLASS()
class GGJ2023_API AxAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AxAICharacter();

	AActor* GetTargetActor();
	void SetTargetActor(AActor* NewTarget);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UxSeedlingStateComponent* SeedlingStateComp;
	//UxWorldUserWidget* ActiveWorldWidget;

	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<UUserWidget> BlaBlaWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UxActionComponent* ActionComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//UxAttributeComponent* AttributeComp;

	virtual void BeginPlay() override;
};
