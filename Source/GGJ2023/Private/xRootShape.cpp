// Fill out your copyright notice in the Description page of Project Settings.


#include "xRootShape.h"

#include "xFactionComponent.h"
#include "xCharacter.h"
#include "xAICharacter.h"
#include "xSeedlingStateComponent.h"
#include "xHexGridTile.h"
#include "ProceduralMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"

AxRootShape::AxRootShape()
{
	bIsInitialized = false;

	ProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMeshComp");
	ProceduralMeshComp->bUseComplexAsSimpleCollision = false;
	//ProceduralMeshComp->SetCollisionProfileName(L"RootShape");
	ProceduralMeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	ProceduralMeshComp->SetGenerateOverlapEvents(true);

	SetReplicates(true);
}

void AxRootShape::BeginPlay()
{
	Super::BeginPlay();
	ProceduralMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AxRootShape::OnOverlapBegin);
	ProceduralMeshComp->OnComponentEndOverlap.AddDynamic(this, &AxRootShape::OnOverlapEnd);

}

void AxRootShape::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AxRootShape::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != OwningPlayer && HasAuthority())
	{
		auto* OwnerFactionComp = UxFactionComponent::GetFactionComponentFromActor(GetOwner());
		auto* OtherFactionComp = UxFactionComponent::GetFactionComponentFromActor(OtherActor);
		if (OwnerFactionComp && OtherFactionComp)
		{
			EFaction OwnerFaction = OwnerFactionComp->GetFaction();
			EFaction OtherFaction = OtherFactionComp->GetFaction();

			if (OtherFaction != OwnerFaction)
			{
				if (Cast<AxAICharacter>(OtherActor) || Cast<AxHexGridTile>(OtherActor))
				{
					OtherFactionComp->SetFaction(OwnerFaction);
				}
			}
		}
	}
}

void AxRootShape::Init(AxCharacter* ShapeOwner, TArray<AxAICharacter*> NewSeedlings)
{
	if (!bIsInitialized && HasAuthority())
	{
		OwningPlayer = ShapeOwner;
		auto Old = Seedlings;
		Seedlings = NewSeedlings;
		OnRep_SeedlingsChange(Old);
	}
}

void AxRootShape::InitCollision()
{
	if (!Seedlings.IsEmpty() && !bIsInitialized)
	{
		bIsInitialized = true;

		TArray<FVector> Poop;
		for (auto V : Seedlings)
		{
			if (V)
			{
				Poop.Add(V->GetActorLocation());
			}
			else
			{
				Poop.Add(FVector());
			}
		}

		Poop.Add(Poop[0] + FVector(0, 0, 500));
		Poop.Add(Poop[1] + FVector(0, 0, 500));
		Poop.Add(Poop[2] + FVector(0, 0, 500));

		ProceduralMeshComp->AddCollisionConvexMesh(Poop);
	}
}

void AxRootShape::OnRep_SeedlingsChange(TArray<AxAICharacter*> OldSeedlings)
{
	for (auto Seed : OldSeedlings)
	{
		if (Seed)
		{
			auto* SeedlingStateComp = Cast<UxSeedlingStateComponent>(Seed->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
			SeedlingStateComp->OnSeedlingStateChanged.RemoveDynamic(this, &AxRootShape::OnSeedlingStateChange);
		}
	}

	for (auto Seed : Seedlings)
	{
		if (Seed)
		{
			auto* SeedlingStateComp = Cast<UxSeedlingStateComponent>(Seed->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
			SeedlingStateComp->OnSeedlingStateChanged.AddDynamic(this, &AxRootShape::OnSeedlingStateChange);
		}
	}

	InitCollision();
}

void AxRootShape::OnSeedlingStateChange(AxAICharacter* Seedling, ESeedlingState NewSeedlingState, ESeedlingState OldSeedlingState)
{
	if (NewSeedlingState == SeedlingState_Uprooted)
	{
		ProceduralMeshComp->DestroyComponent();
	}
}

void AxRootShape::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxRootShape, Seedlings);
	DOREPLIFETIME(AxRootShape, OwningPlayer);
}
