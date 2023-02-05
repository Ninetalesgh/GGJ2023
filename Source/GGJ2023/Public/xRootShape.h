// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "xRootShape.generated.h"

class UProceduralMeshComponent;

UCLASS()
class GGJ2023_API AxRootShape : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AxRootShape();

	UFUNCTION()
	void SetVertices(TArray<FVector> NewVertices);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitCollision();

	UPROPERTY()
	bool bIsInitialized;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root Pattern")
	UProceduralMeshComponent* ProceduralMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_VerticesChange", Category = "Root Pattern")
	TArray<FVector> Vertices;

	UFUNCTION()
	void OnRep_VerticesChange(TArray<FVector> OldVertices);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:

};
