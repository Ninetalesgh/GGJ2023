// Fill out your copyright notice in the Description page of Project Settings.


#include "xPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AxPlayerState::AxPlayerState()
{
	RepData.Faction = Faction_Unassigned;
	RepData.Variation = FactionVariation_0;
}

AxPlayerState* AxPlayerState::GetPlayerStateFromActor(AActor* FromActor)
{
	if (ACharacter* Character = Cast<ACharacter>(FromActor))
	{
		return Cast<AxPlayerState>(Character->GetPlayerState());
	}
	else if (APlayerController* Controller = Cast<APlayerController>(FromActor))
	{
		return Controller->GetPlayerState<AxPlayerState>();
	}
	else if (AxPlayerState* State = Cast<AxPlayerState>(FromActor))
	{
		return State;
	}
	return nullptr;
}

EFaction AxPlayerState::GetFaction() const
{
	return RepData.Faction;
}

EFactionVariation AxPlayerState::GetFactionVariation() const
{
	return RepData.Variation;
}

void AxPlayerState::SetFaction(EFaction NewFaction)
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

void AxPlayerState::SetFactionVariation(EFactionVariation NewVariation)
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

void AxPlayerState::OnRep_FactionChange(FFactionRepData OldRepData)
{
	OnFactionChanged.Broadcast(this, RepData.Faction, RepData.Variation, OldRepData.Faction, OldRepData.Variation);
}

void AxPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxPlayerState, RepData);
}
