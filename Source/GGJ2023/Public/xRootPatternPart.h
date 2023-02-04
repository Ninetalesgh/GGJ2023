// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "xRootPatternPart.generated.h"

//class UProceduralMeshComponent;

UCLASS()
class GGJ2023_API AxRootPatternPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AxRootPatternPart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	//UProceduralMeshComponent* ProceduralMeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
