// Fill out your copyright notice in the Description page of Project Settings.


#include "xGameMode.h"
#include "xPlayerState.h"
#include "xCharacter.h"
#include "xAICharacter.h"

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

	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	AActor* StartSpot = FindPlayerStart(NewPlayer);
	FRotator SpawnRotation = StartSpot->GetActorRotation();

	if (NewPlayer->GetPawn() != nullptr)
	{
	}
	else if (GetDefaultPawnClassForController(NewPlayer) != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = SpawnDefaultPawnFor(NewPlayer, StartSpot);
		if (IsValid(NewPawn))
		{
			NewPlayer->SetPawn(NewPawn);	
		}

		auto* Char = Cast<AxCharacter>(NewPawn);

		if (FollowerClass->IsChildOf<AxAICharacter>())
		{
			FVector SpawnLocation = Char->GetActorLocation();
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Char->Follower = Cast<AxAICharacter>(GetWorld()->SpawnActor<AActor>(FollowerClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams));
		}
	}

	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		// Tell the start spot it was used
		InitStartSpot(StartSpot, NewPlayer);

		//This possesses the pawn
		FinishRestartPlayer(NewPlayer, SpawnRotation);
	}




	//Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}
