// Fill out your copyright notice in the Description page of Project Settings.


#include "xSeedlingStateComponent.h"
#include "xAICharacter.h"
#include "xCharacter.h"
#include "Net/UnrealNetwork.h"

UxSeedlingStateComponent::UxSeedlingStateComponent()
{
	RepData.Faction = Faction_Unassigned;
	RepData.Variation = FactionVariation_0;
	SetIsReplicatedByDefault(true);
}

void UxSeedlingStateComponent::SetOwningPlayer(AxCharacter* NewOwner)
{
	if (ensure(GetOwner()->HasAuthority()))
	{
		auto* PreviousOwner = OwningPlayer;
		OwningPlayer = NewOwner;
		if (PreviousOwner != NewOwner)
		{
			OnRep_OwningPlayerChange(PreviousOwner);
		}
	}
}

void UxSeedlingStateComponent::OnRep_OwningPlayerChange(AxCharacter* PreviousOwner)
{
	OnOwningPlayerChanged.Broadcast(Cast<AxAICharacter>(GetOwner()), PreviousOwner, OwningPlayer);
}



EFaction UxSeedlingStateComponent::GetFaction()
{
	return RepData.Faction;
}

EFactionVariation UxSeedlingStateComponent::GetFactionVariation()
{
	return RepData.Variation;
}

void UxSeedlingStateComponent::SetFaction(EFaction NewFaction)
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

void UxSeedlingStateComponent::OnRep_FactionChange(FFactionRepData OldRepData)
{
	OnFactionChanged.Broadcast(GetOwner(), RepData.Faction, RepData.Variation, OldRepData.Faction, OldRepData.Variation);
}

void UxSeedlingStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UxSeedlingStateComponent, OwningPlayer);
	DOREPLIFETIME(UxSeedlingStateComponent, RepData);
}
