// Fill out your copyright notice in the Description page of Project Settings.


#include "xPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

AxPlayerState::AxPlayerState()
{
	RepData.Faction = Faction_Unassigned;
	RepData.Variation = FactionVariation_0;
}

EFaction AxPlayerState::GetFaction()
{
	return RepData.Faction;
}

EFactionVariation AxPlayerState::GetFactionVariation()
{
	return RepData.Variation;
}

void AxPlayerState::SetFaction(EFaction NewFaction)
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

void AxPlayerState::OnRep_FactionChange(FFactionRepData OldRepData)
{
	OnFactionChanged.Broadcast(GetPlayerController()->GetPawn(), RepData.Faction, RepData.Variation, OldRepData.Faction, OldRepData.Variation);
}

void AxPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxPlayerState, RepData);
}
