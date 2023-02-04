// Fill out your copyright notice in the Description page of Project Settings.


#include "xGameMode.h"
#include "xPlayerState.h"
#include "xCharacter.h"
#include "xAICharacter.h"
#include "xFactionComponent.h"
#include "xRootPatternPart.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "TimerManager.h"


AxGameMode::AxGameMode()
{
	MaxSeedlingsPerPlayer = 7;
	MaxSeedlingsTotal = 30;
	MaxUnassignedSeedlings = 5;
}

void AxGameMode::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnSeedlings, this, &AxGameMode::SpawnSeedlingsTimerElapsed, SpawnTimerInterval, true);

}

void AxGameMode::GenerateHexGrid()
{

}

void AxGameMode::DrawSeedlingTriangle(AxCharacter* InstigatorPlayer, TArray<AxAICharacter*> Seedlings)
{
	//TODO set states on all hexes in triangle

}

void AxGameMode::UprootSeedling(AxAICharacter* Seedling)
{
	//destroy triangle without destroying overlapping ones?
	// 
	//uproot all seedlings that are now alone?
}

TArray<AxAICharacter*> AxGameMode::GetAllSeedlingsOfFaction(EFaction Faction)
{
	TArray<AxAICharacter*> SeedlingsOfFaction;

	for (TActorIterator<AxAICharacter> It(GetWorld()); It; ++It)
	{
		AxAICharacter* Seedling = *It;
		UxFactionComponent* FactionComp = Cast<UxFactionComponent>(Seedling->GetComponentByClass(UxFactionComponent::StaticClass()));
		if (FactionComp && FactionComp->GetFaction() == Faction_Unassigned)
		{
			SeedlingsOfFaction.Add(Seedling);
		}
	}

	return SeedlingsOfFaction;
}

void AxGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	AxPlayerState* PS = NewPlayer->GetPlayerState<AxPlayerState>();
	if (PS)
	{
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
			Char->Follower = Cast<AxAICharacter>(GetWorld()->SpawnActor<AActor>(FollowerClass, SpawnLocation + FVector(0, 0, 200), FRotator::ZeroRotator, SpawnParams));
			Char->Follower->SpawnDefaultController();
		
			//auto* Root = Cast<AxRootPatternPart>(GetWorld()->SpawnActor<AActor>(RootPatternPartClass, SpawnLocation + FVector(0,200,0), FRotator::ZeroRotator, SpawnParams));

			TArray<AActor*> Poop;
			Poop.Add(Char);
			Poop.Add(Char->Follower);
			//Poop.Add(Root);

			//Root->SetShape(Poop);
		
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
}

void AxGameMode::SpawnSeedlingsTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnSeedlingQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AxGameMode::OnQueryCompleted);

	}
}

void AxGameMode::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Seedling EQS Query Failed!"));
		return;
	}

	
	int32 NrOfUndefinedSeedlings = 0;
	for (TActorIterator<AxAICharacter> It(GetWorld()); It; ++It)
	{
		AxAICharacter* Seedling = *It;

		UxFactionComponent* FactionComp = Cast<UxFactionComponent>(Seedling->GetComponentByClass(UxFactionComponent::StaticClass()));
		if (FactionComp && FactionComp->GetFaction() == Faction_Unassigned)
		{
			NrOfUndefinedSeedlings++;
		}
	}

	float MaxSeedlingCount = 30.0f;
		if (NrOfUndefinedSeedlings >= MaxUnassignedSeedlings)
		{
			return;
		}
	
	if (DifficultyCurve)
	{
		//MaxSeedlingCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(FollowerClass, Locations[0], FRotator::ZeroRotator);
	}

}
