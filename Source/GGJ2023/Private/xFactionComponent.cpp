// Fill out your copyright notice in the Description page of Project Settings.


#include "xFactionComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

UxFactionComponent::UxFactionComponent()
{
	RepData.Faction = Faction_Unassigned;
	RepData.Variation = FactionVariation_0;
	SetIsReplicatedByDefault(true);
}

EFaction UxFactionComponent::GetFaction() const
{
	return RepData.Faction;
}

EFactionVariation UxFactionComponent::GetFactionVariation() const
{
	return RepData.Variation;
}

void UxFactionComponent::SetFaction(EFaction NewFaction)
{
	if (GetOwner()->HasAuthority())
	{
		auto OldRepData = RepData;
		RepData.Faction = NewFaction;
		if (RepData != OldRepData)
		{
			OnRep_FactionChange(OldRepData);
		}
	}
}

void UxFactionComponent::SetFactionVariation(EFactionVariation NewVariation)
{
	if (GetOwner()->HasAuthority())
	{
		auto OldRepData = RepData;
		RepData.Variation = NewVariation;
		if (RepData != OldRepData)
		{
			OnRep_FactionChange(OldRepData);
		}
	}
}

void UxFactionComponent::OnRep_FactionChange(FFactionRepData OldRepData)
{
	OnFactionChanged.Broadcast(GetOwner(), RepData.Faction, RepData.Variation, OldRepData.Faction, OldRepData.Variation);
}

void UxFactionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UxFactionComponent, RepData);
}
