// Fill out your copyright notice in the Description page of Project Settings.


#include "xHexGridTile.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AxHexGridTile::AxHexGridTile()
{
 	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetReplicates(true);

	RepData.Faction = Faction_Unassigned;
	RepData.Variation = FactionVariation_0;
}

void AxHexGridTile::BeginPlay()
{
	Super::BeginPlay();
	
}

EFaction AxHexGridTile::GetFaction() const
{
	return RepData.Faction;
}

EFactionVariation AxHexGridTile::GetFactionVariation() const
{
	return RepData.Variation;
}

void AxHexGridTile::SetFaction(EFaction NewFaction)
{
	if (ensure(GetOwner()->HasAuthority()))
	{
		auto OldRepData = RepData;
		RepData.Faction = NewFaction;
		if (RepData != OldRepData)
		{
			OnRep_FactionChange(OldRepData);
		}
	}
}

void AxHexGridTile::SetFactionVariation(EFactionVariation NewVariation)
{
	if (ensure(GetOwner()->HasAuthority()))
	{
		auto OldRepData = RepData;
		RepData.Variation = NewVariation;
		if (RepData != OldRepData)
		{
			OnRep_FactionChange(OldRepData);
		}
	}
}

void AxHexGridTile::OnRep_FactionChange(FFactionRepData OldRepData)
{
	OnFactionChanged.Broadcast(this, RepData.Faction, RepData.Variation, OldRepData.Faction, OldRepData.Variation);
}

void AxHexGridTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxHexGridTile, RepData);
}
