// Fill out your copyright notice in the Description page of Project Settings.


#include "xRootShape.h"

#include "xFactionComponent.h"
#include "xCharacter.h"
#include "xAICharacter.h"
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
	if (OtherActor != GetOwner() && GetOwner()->HasAuthority())
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

void AxRootShape::SetVertices(TArray<FVector> NewVertices)
{
	if (bIsInitialized || !GetOwner()->HasAuthority())
	{
		return;
	}

	Vertices = NewVertices;
}

void AxRootShape::InitCollision()
{
	if (!Vertices.IsEmpty() && !bIsInitialized)
	{
		bIsInitialized = true;
		FVector N = Vertices.Last(0);
		for (auto V : Vertices)
		{
			DrawDebugLine(GetWorld(), V, N, FColor::Blue, false, 10.0f, 0, 2.0f);
			N = V;
		}

		TArray<FVector> Poop = Vertices;

		Poop.Add(Poop[0] + FVector(0, 0, 500));
		Poop.Add(Poop[1] + FVector(0, 0, 500));
		Poop.Add(Poop[2] + FVector(0, 0, 500));

		ProceduralMeshComp->AddCollisionConvexMesh(Poop);
	}
}

void AxRootShape::OnRep_VerticesChange(TArray<FVector> OldVertices)
{
	InitCollision();
}

void AxRootShape::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxRootShape, Vertices);
}
