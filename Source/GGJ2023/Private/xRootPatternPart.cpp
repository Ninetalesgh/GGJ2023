// Fill out your copyright notice in the Description page of Project Settings.


#include "xRootPatternPart.h"
#include "xFactionComponent.h"
#include "ProceduralMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

AxRootPatternPart::AxRootPatternPart()
{
	ProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMeshComp");
	ProceduralMeshComp->SetGenerateOverlapEvents(true);
	ProceduralMeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AxRootPatternPart::BeginPlay()
{
	Super::BeginPlay();
	ProceduralMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AxRootPatternPart::OnOverlap);
}

void AxRootPatternPart::SetShape(TArray<AActor*> Seedlings)
{
	if (Seedlings.Num())
	{	
		TArray<FVector> Vertices;

		//FVector Next = Seedlings[Seedlings.Num() - 1]->GetActorLocation();
		FVector Next = FVector(0,0,0);
		
		for (auto Seedling : Seedlings)
		{
			FVector HalfHeight = FVector(0, 0, 50);
			FVector Position = Seedling->GetActorLocation();
			Vertices.Add(Position + HalfHeight);
			Vertices.Add(Position - HalfHeight);
			DrawDebugLine(GetWorld(), Next, Position, FColor::Green, false, 0.0f, 0, 5.0f);
			Next = Position;
		}

		ProceduralMeshComp->AddCollisionConvexMesh(Vertices);
	}
}

void AxRootPatternPart::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//auto* Owner = OverlappedComponent->GetOwner();
	//auto* FactionComp = Cast<UxFactionComponent*>(Owner->GetComponentByClass(UxFactionComponent::StaticClass()));

}