// Fill out your copyright notice in the Description page of Project Settings.


#include "xInteractionComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "xAICharacter.h"
#include "xCharacter.h"
#include "xActionComponent.h"
#include "xSeedlingStateComponent.h"
#include "xFactionComponent.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Net/UnrealNetwork.h"
#include "../GGJ2023.h"

UxInteractionComponent::UxInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceDistance = 750.f;
	TraceRadius = 100.0f;
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
		LogOnScreen(this, L"No planted seedling in focus to uproot.", FColor::Red);
		return;
	}

	AxAICharacter* Seedling = Cast<AxAICharacter>(InFocus);
	if (Seedling)
	{
		auto* Char = Cast<AxCharacter>(GetOwner());
		UxFactionComponent* FC = UxFactionComponent::GetFactionComponentFromActor(Seedling);
		UxFactionComponent* OwnerFC = UxFactionComponent::GetFactionComponentFromActor(GetOwner());
		FC->SetFaction(OwnerFC->GetFaction());

		auto* SSC = Cast<UxSeedlingStateComponent>(Seedling->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
		SSC->SetSeedlingState(SeedlingState_Uprooted);

		if (Char)
		{
			Char->AppendSeedling(Seedling);
			SSC->SetOwningPlayer(Char);
			auto* Last = Char->GetLastSeedling();

			if (Last)
			{
				Seedling->SetNext(Last);
			}
			else
			{
				Seedling->SetNext(Char);
			}
		}

		OnUproot.Broadcast(Char,Seedling);
		MulticastUproot(Char, Seedling);
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

	FocusedActor = nullptr;
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (auto* Seedling = Cast<AxAICharacter>(HitActor))
		{
			auto* SSC = Cast<UxSeedlingStateComponent>(Seedling->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
			if (SSC->GetSeedlingState() == SeedlingState_Planted)
			{
				FocusedActor = HitActor;
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, FColor::Green, false, 0.0f);
			}
			else
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, FColor::Red, false, 0.0f);
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

}


void UxInteractionComponent::MulticastUproot_Implementation(AxCharacter* Player, AxAICharacter* Seedling)
{
	OnUproot.Broadcast(Player,Seedling);
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

