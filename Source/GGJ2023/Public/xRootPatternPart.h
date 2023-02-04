// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "xRootPatternPart.generated.h"

class UProceduralMeshComponent;
class AActor;

UCLASS()
class GGJ2023_API AxRootPatternPart : public AActor
{
	GENERATED_BODY()
	
public:	
	AxRootPatternPart();

	UFUNCTION(BlueprintCallable, Category="Root Pattern")
	void SetShape(TArray<AActor*> Seedlings);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root Pattern")
	UProceduralMeshComponent* ProceduralMeshComp;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
