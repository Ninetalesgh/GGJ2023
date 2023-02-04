// Fill out your copyright notice in the Description page of Project Settings.


#include "xPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

AxPlayerState::AxPlayerState()
{
	Faction = Faction_Unassigned;
}

void AxPlayerState::SetFaction(EFaction NewFaction)
{
	if (ensure(GetOwner()->HasAuthority()))
	{
		auto PreviousFaction = NewFaction;
		Faction = NewFaction;
		if (PreviousFaction != NewFaction)
		{
			OnRep_FactionChange(PreviousFaction);
		}
	}
}

void AxPlayerState::OnRep_FactionChange(EFaction PreviousFaction)
{
	OnFactionChanged.Broadcast(Cast<ACharacter>(GetPlayerController()->GetPawn()), PreviousFaction, Faction);
}

void AxPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxPlayerState, Faction);
}
