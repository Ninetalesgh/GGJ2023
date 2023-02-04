// Fill out your copyright notice in the Description page of Project Settings.


#include "xSeedlingStateComponent.h"
#include "xAICharacter.h"
#include "xCharacter.h"
#include "Net/UnrealNetwork.h"

UxSeedlingStateComponent::UxSeedlingStateComponent()
{
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

void UxSeedlingStateComponent::OnRep_SeedlingStateChange(ESeedlingState PreviousState)
{
	OnSeedlingStateChanged.Broadcast(Cast<AxAICharacter>(GetOwner()), State,PreviousState);
}

void UxSeedlingStateComponent::OnRep_OwningPlayerChange(AxCharacter* PreviousOwner)
{
	OnOwningPlayerChanged.Broadcast(Cast<AxAICharacter>(GetOwner()), OwningPlayer, PreviousOwner);
}

void UxSeedlingStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UxSeedlingStateComponent, OwningPlayer);
	DOREPLIFETIME(UxSeedlingStateComponent, State);
}
