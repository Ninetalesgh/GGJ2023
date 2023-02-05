// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "xAICharacter.generated.h"

class UxActionComponent;
class UxAttributeComponent;
class AxCharacter;
class UxSeedlingStateComponent;
class UBillboardComponent;
class UxCameraFacingFlipBookComponent;

//class UUserWidget;
//class UxWorldUserWidget;
class UxFactionComponent;


UCLASS()
class GGJ2023_API AxAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AxAICharacter();

	ACharacter* GetNext();
	AxAICharacter* GetPrevious();

	void SetNext(ACharacter* Next);
	void SetPrevious(AxAICharacter* Previous);
protected:



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	UxCameraFacingFlipBookComponent* BodyComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UxSeedlingStateComponent* SeedlingStateComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UxFactionComponent* FactionComp;
	//UxWorldUserWidget* ActiveWorldWidget;

	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<UUserWidget> BlaBlaWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UxActionComponent* ActionComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//UxAttributeComponent* AttributeComp;

	virtual void BeginPlay() override;
};
