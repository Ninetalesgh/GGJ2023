// Fill out your copyright notice in the Description page of Project Settings.


#include "xPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AxPlayerState::AxPlayerState()
{
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

void AxPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
