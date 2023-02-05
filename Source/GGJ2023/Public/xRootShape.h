// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "xSeedlingStateComponent.h"
#include "xRootShape.generated.h"

class UProceduralMeshComponent;
class AxAICharacter;
class AxCharacter;

UCLASS()
class GGJ2023_API AxRootShape : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AxRootShape();

	UFUNCTION()
	void Init(AxCharacter* ShapeOwner, TArray<AxAICharacter*> NewSeedlings);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitCollision();

	UPROPERTY()
	bool bIsInitialized;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root Pattern")
	UProceduralMeshComponent* ProceduralMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_SeedlingsChange", Category = "Root Pattern")
	TArray<AxAICharacter*> Seedlings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Root Pattern")
	AxCharacter* OwningPlayer;
	
	UFUNCTION()
	void OnRep_SeedlingsChange(TArray<AxAICharacter*> OldSeedlings);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnSeedlingStateChange(AxAICharacter* Seedling, ESeedlingState NewSeedlingState, ESeedlingState OldSeedlingState);
};
