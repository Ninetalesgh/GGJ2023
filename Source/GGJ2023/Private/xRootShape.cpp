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
	PrimaryActorTick.bCanEverTick = true;
	bIsInitialized = false;
	bHasTriggered = false;
	ProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMeshComp");
	ProceduralMeshComp->bUseComplexAsSimpleCollision = false;
	ProceduralMeshComp->SetCollisionProfileName(L"RootShape");
	ProceduralMeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	ProceduralMeshComp->SetGenerateOverlapEvents(true);

	ProceduralMeshComp->SetCollisionObjectType(ECC_Pawn);
	ProceduralMeshComp->SetCollisionResponseToChannel(ECC_WorldDynamic,ECollisionResponse::ECR_Overlap);

	SetReplicates(true);
}

void AxRootShape::BeginPlay()
{
	Super::BeginPlay();
	ProceduralMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AxRootShape::OnOverlapBegin);
	ProceduralMeshComp->OnComponentEndOverlap.AddDynamic(this, &AxRootShape::OnOverlapEnd);
	PrimaryActorTick.SetTickFunctionEnable(true);
}

void AxRootShape::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

EFaction AxRootShape::GetFaction()
{
	auto* OwnerFactionComp = UxFactionComponent::GetFactionComponentFromActor(OwningPlayer);
	return OwnerFactionComp ? OwnerFactionComp->GetFaction() : Faction_Unassigned;
}

void AxRootShape::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != OwningPlayer && HasAuthority())
	{
		auto* OwnerFactionComp = UxFactionComponent::GetFactionComponentFromActor(OwningPlayer);
		auto* OtherFactionComp = UxFactionComponent::GetFactionComponentFromActor(OtherActor);
		if (OwnerFactionComp && OtherFactionComp)
		{
			EFaction OwnerFaction = OwnerFactionComp->GetFaction();
			EFaction OtherFaction = OtherFactionComp->GetFaction();

			if (OtherFaction != OwnerFaction)
			{
				if (Cast<AxHexGridTile>(OtherActor))
				{
					OtherFactionComp->SetFaction(OwnerFaction);
				}
			}
		}
	}
}

void AxRootShape::Tick(float DeltaSeconds)
{
	if (!bHasTriggered)
	{
		TArray<AActor*> Overlappers;
		ProceduralMeshComp->GetOverlappingActors(Overlappers);
		if (!Overlappers.IsEmpty())
		{
			//bHasTriggered = true;
			//PrimaryActorTick.SetTickFunctionEnable(false);
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

		//auto* ShapeOwnerFC = UxFactionComponent::GetFactionComponentFromActor(ShapeOwner);

		//TSet<AActor*> OverlappingActors;
		//ProceduralMeshComp->GetOverlappingActors(OverlappingActors);
		//for (auto A : OverlappingActors)
		//{
		//	if (Cast<AxHexGridTile>(A))
		//	{
		//		auto* FC = UxFactionComponent::GetFactionComponentFromActor(A);
		//		if (FC)
		//		{
		//			FC->SetFaction(ShapeOwnerFC->GetFaction());
		//		}
		//	}
		//}

		InitCollision();
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
				Poop.Add(V->GetActorLocation() - FVector(0,0,200));
			}
			else
			{
				Poop.Add(FVector());
			}
		}

		Poop.Add(Poop[0] + FVector(0, 0, 200));
		Poop.Add(Poop[1] + FVector(0, 0, 200));
		Poop.Add(Poop[2] + FVector(0, 0, 200));

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
			if (SeedlingStateComp)
			{
				SeedlingStateComp->OnSeedlingStateChanged.RemoveDynamic(this, &AxRootShape::OnSeedlingStateChange);
			}
		}
	}

	for (auto Seed : Seedlings)
	{
		if (Seed)
		{
			auto* SeedlingStateComp = Cast<UxSeedlingStateComponent>(Seed->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
			if (SeedlingStateComp)
			{
				SeedlingStateComp->OnSeedlingStateChanged.AddDynamic(this, &AxRootShape::OnSeedlingStateChange);
			}
		}
	}
}

void AxRootShape::OnSeedlingStateChange(AxAICharacter* Seedling, ESeedlingState NewSeedlingState, ESeedlingState OldSeedlingState)
{
	if (HasAuthority())
	{
		if (NewSeedlingState == SeedlingState_Uprooted)
		{
			for (auto Seed : Seedlings)
			{
				if (Seed)
				{
					auto* SeedlingStateComp = Cast<UxSeedlingStateComponent>(Seed->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
					if (SeedlingStateComp)
					{
						SeedlingStateComp->OnSeedlingStateChanged.RemoveDynamic(this, &AxRootShape::OnSeedlingStateChange);
					}
				}
			}
			Seedlings.Empty();

			TSet<AActor*> OverlappingActors;
			ProceduralMeshComp->GetOverlappingActors(OverlappingActors);
			for (auto A : OverlappingActors)
			{
				if (Cast<AxHexGridTile>(A))
				{
					auto* FC = UxFactionComponent::GetFactionComponentFromActor(A);
					if (FC)
					{
						FC->SetFaction(Faction_Unassigned);
					}
				}
			}

			GetWorld()->DestroyActor(this);
		}
	}
}

void AxRootShape::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxRootShape, Seedlings);
	DOREPLIFETIME(AxRootShape, OwningPlayer);
}