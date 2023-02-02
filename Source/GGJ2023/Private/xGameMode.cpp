// Fill out your copyright notice in the Description page of Project Settings.


#include "xGameMode.h"
#include "xPlayerState.h"

AxGameMode::AxGameMode()
{
	//PlayerStateClass = AxPlayerState::StaticClass();
	//SlotName = "SaveGame01";
}


void AxGameMode::StartPlay()
{
	Super::StartPlay();
}


void AxGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	AxPlayerState* PS = NewPlayer->GetPlayerState<AxPlayerState>();
	if (PS)
	{
		//PS->PlayerIndex = PlayerCount++;
		//PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}
