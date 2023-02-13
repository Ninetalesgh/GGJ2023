// Fill out your copyright notice in the Description page of Project Settings.


#include "xAction_Plant.h"
#include "TimerManager.h"
#include "xCharacter.h"
#include "xAICharacter.h"
#include "xGameMode.h"
#include "xSeedlingStateComponent.h"
#include "xRootShape.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "../GGJ2023.h"

UxAction_Plant::UxAction_Plant()
{
	ActionAnimDelay = 0.2f;
}

void UxAction_Plant::ServerOnlyActionPart_Implementation(AActor* InstigatorActor)
{
	Super::ServerOnlyActionPart_Implementation(InstigatorActor);

	AxCharacter* Character = Cast<AxCharacter>(InstigatorActor);
	if (Character)
	{
		if (ActionAnimDelay >= 0.001f)
		{
			FTimerHandle TimerHandle_ActionDelay;
			FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UxAction_Plant::ActionDelay_Elapsed, Character);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_ActionDelay, Delegate, ActionAnimDelay, false);
		}
		else
		{
			ActionDelay_Elapsed(Character);
		}
	}
}

void UxAction_Plant::ActionDelay_Elapsed(AxCharacter* InstigatorCharacter)
{
	auto* GM = Cast<AxGameMode>(InstigatorCharacter->GetWorld()->GetAuthGameMode());
	auto* FC = UxFactionComponent::GetFactionComponentFromActor(InstigatorCharacter);

	if (GM && FC)
	{
		auto Factoids = GM->GetAllPlantedSeedlingsOfFaction(FC->GetFaction());
		
		auto* NextSeedling = InstigatorCharacter->GetNextSeedling();
		
		if (auto* NextSeedlingFactionComp = UxFactionComponent::GetFactionComponentFromActor(NextSeedling))
		{
			NextSeedlingFactionComp->SetFaction(FC->GetFaction());
			auto* SeedlingStateComp = Cast<UxSeedlingStateComponent>(NextSeedling->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
			if (SeedlingStateComp)
			{
				SeedlingStateComp->SetSeedlingState(SeedlingState_Planted);
				NextSeedling->SetNext(nullptr);
			}
			
			NextSeedling->SetActorLocation(InstigatorCharacter->GetActorLocation());

			//Reorganize walking seedlings
			InstigatorCharacter->MakeSnake();

			//Rooted poopers
			if (Factoids.Num() > 1)
			{
				float RadSquared= InstigatorCharacter->RootRadius* InstigatorCharacter->RootRadius;
				float BestDistances[2] = {9999999999.0f,9999999999.0f };
			
				AxAICharacter* Seedlings[2];

				bool OneInRange = false;

				for (auto F : Factoids)
				{
					float Dist = (F->GetActorLocation() - InstigatorCharacter->GetActorLocation()).SquaredLength();

					if (Dist < RadSquared)
					{
						OneInRange = true;
					}

					if (Dist < BestDistances[0])
					{
						if (BestDistances[0] < BestDistances[1])
						{
							BestDistances[1] = BestDistances[0];
							Seedlings[1] = Seedlings[0];
						}

						BestDistances[0] = Dist;
						Seedlings[0] = F;
					}
					else if (Dist < BestDistances[1])
					{
						BestDistances[1] = Dist;
						Seedlings[1] = F;
					}
				}

				if (OneInRange)
				{
					TArray<AxAICharacter*> RootedSeedlings;
			
					RootedSeedlings.Add(NextSeedling);
					RootedSeedlings.Add(Seedlings[1]);
					RootedSeedlings.Add(Seedlings[0]);

					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					auto* NewRootShape = Cast<AxRootShape>(GetWorld()->SpawnActor<AActor>(RootShapeClass, FVector(0,0,0), FRotator::ZeroRotator, SpawnParams));
					NewRootShape->Init(InstigatorCharacter, RootedSeedlings);
				}
				else
				{
					LogOnScreen(this, L"Planted Seedling - No Planted Seedling in range.");
				}
			}
			else if (Factoids.Num() == 1)
			{
				LogOnScreen(this, L"Planted Seedling - One Other seedling planted.");
			}
			else
			{
				LogOnScreen(this, L"Planted Seedling - No Seedling planted yet.");
			}
		}
		else
		{
			LogOnScreen(this, L"No uprooted seedlings for your faction :(.");
		}
	}

	StopAction(InstigatorCharacter);
}
