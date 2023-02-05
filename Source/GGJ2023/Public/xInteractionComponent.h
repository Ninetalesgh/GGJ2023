// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "xInteractionComponent.generated.h"

class AxCharacter;
class AxAICharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUproot, AxCharacter*, Player, AxAICharacter*, Seedling);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GGJ2023_API UxInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UxInteractionComponent();

	void Interact();

	UPROPERTY(BlueprintAssignable, Category = "Uproot")
	FOnUproot OnUproot;

protected:
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestSeedling();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUproot(AxCharacter* Player, AxAICharacter* Seedling);

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
