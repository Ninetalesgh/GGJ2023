// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "xCharacter.generated.h"

class UxActionComponent;
class UxAttributeComponent;
class UInputMappingContext;
class UInputAction;
class UBillboardComponent;
class UxCameraFacingFlipBookComponent;

class AxAICharacter;

UCLASS()
class GGJ2023_API AxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AxCharacter();

protected:
	virtual void BeginPlay() override;

	void Plant();

	void MoveUp(float Value);
	void MoveRight(float Value);

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	AxAICharacter* Follower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UxActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	UBillboardComponent* HatComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	UxCameraFacingFlipBookComponent* BodyComp;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
