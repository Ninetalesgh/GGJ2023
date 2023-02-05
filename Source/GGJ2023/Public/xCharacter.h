// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "xCharacter.generated.h"

class UxActionComponent;
class UxAttributeComponent;
class UInputMappingContext;
class UInputAction;
class UxCameraFacingFlipBookComponent;
class UxFactionComponent;
class AxAICharacter;
class UxInteractionComponent;

UCLASS()
class GGJ2023_API AxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AxCharacter();

protected:
	virtual void BeginPlay() override;

	void Plant();
	void Uproot();

	void MoveUp(float Value);
	void MoveRight(float Value);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float RootRadius;

	AxAICharacter* GetNextSeedling();
	AxAICharacter* GetLastSeedling();

	UFUNCTION(BlueprintCallable)
	void AppendSeedling(AxAICharacter* Seedling);

	UFUNCTION(BlueprintCallable)
	void MakeSnake();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	AxAICharacter* NextFollower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	AxAICharacter* LastFollower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UxActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	UxCameraFacingFlipBookComponent* BodyComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	UxInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UxFactionComponent* FactionComp;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
