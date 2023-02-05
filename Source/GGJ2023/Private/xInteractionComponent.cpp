// Fill out your copyright notice in the Description page of Project Settings.


#include "xInteractionComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "xAICharacter.h"
#include "xSeedlingStateComponent.h"
#include "xFactionComponent.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

UxInteractionComponent::UxInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceDistance = 500.f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_Pawn;
}


void UxInteractionComponent::Interact()
{
	ServerInteract(FocusedActor);
}

void UxInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (!InFocus)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No seedling to uproot.");
		return;
	}

	AxAICharacter* Seedling = Cast<AxAICharacter>(InFocus);
	if (Seedling)
	{
		UxFactionComponent* FC = UxFactionComponent::GetFactionComponentFromActor(Seedling);
		UxFactionComponent* OwnerFC = UxFactionComponent::GetFactionComponentFromActor(GetOwner());
		FC->SetFaction(OwnerFC->GetFaction());

		auto* SSC = Cast<UxSeedlingStateComponent>(Seedling->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
		SSC->SetSeedlingState(SeedlingState_Uprooted);
	}
}

void UxInteractionComponent::FindBestSeedling()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation = GetOwner()->GetActorLocation();
	FVector End = EyeLocation + (GetOwner()->GetActorForwardVector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	for (FHitResult Hit : Hits)
	{
		//if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (auto* Seedling = Cast<AxAICharacter>(HitActor))
		{
			auto* SSC = Cast<UxSeedlingStateComponent>(Seedling->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
			if (SSC->GetSeedlingState() == SeedlingState_Planted)
			{
				FocusedActor = HitActor;
			}
		}
	}

	if (FocusedActor)
	{
		//TODO hover effect?
	}
	else
	{
		//TODO remove hover effect
	}


	//if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 0.0f, 0, 2.0f);
	}
}


void UxInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	APawn const* const MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestSeedling();
	}
}

