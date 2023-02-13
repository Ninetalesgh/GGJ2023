// Fill out your copyright notice in the Description page of Project Settings.


#include "xHexGridTile.h"
#include "xFactionComponent.h"
#include "xRootShape.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AxHexGridTile::AxHexGridTile()
{
 	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	FactionComp = CreateDefaultSubobject<UxFactionComponent>("FactionComp");

	MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	MeshComp->SetGenerateOverlapEvents(true);
}

void AxHexGridTile::BeginPlay()
{
	Super::BeginPlay();	
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AxHexGridTile::OnOverlapBegin);
	MeshComp->OnComponentEndOverlap.AddDynamic(this, &AxHexGridTile::OnOverlapEnd);
}

void AxHexGridTile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (auto* boob = Cast<AxRootShape>(OtherActor))
		{
			FactionComp->SetFaction(boob->GetFaction());
		}
	}
}

void AxHexGridTile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	 
}


void AxHexGridTile::ServerSetFaction_Implementation(EFaction Faction)
{
	UxFactionComponent::GetFactionComponentFromActor(this)->SetFaction(Faction);
}