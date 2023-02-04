// Fill out your copyright notice in the Description page of Project Settings.


#include "xAction_Plant.h"
#include "TimerManager.h"
#include "xCharacter.h"
#include "xAICharacter.h"
#include "xSeedlingStateComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraActor.h"

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
		//UGameplayStatics::SpawnEmitterAttached(ActionEffect, Character->GetMesh(),HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		Character->PlayAnimMontage(ActionAnimation);
		

		FTimerHandle TimerHandle_ActionDelay;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UxAction_Plant::ActionDelay_Elapsed, Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ActionDelay, Delegate, ActionAnimDelay, false);
		
	}
}

void UxAction_Plant::ActionDelay_Elapsed(AxCharacter* InstigatorCharacter)
{
	auto* Root = Cast<AxRootPatternPart>(GetWorld()->SpawnActor<AActor>(RootPatternPartClass, InstigatorCharacter->GetActorLocation() + FVector(500,0,200), FRotator::ZeroRotator));

	//TArray<AActor*> beep;

	//TArray<AActor*> Cameras;
	//UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), Cameras);


	/*beep.Add(Cameras[0]);
	beep.Add(InstigatorCharacter);
	beep.Add(InstigatorCharacter->Follower);

	Root->SetShape(beep);*/

	//TODO fetch AI follower here and plant it in the ground in front of player
	if (InstigatorCharacter->Follower)
	{
		InstigatorCharacter->Follower->SetActorTransform(InstigatorCharacter->GetActorTransform());
		auto* SeedlingState = Cast<UxSeedlingStateComponent>(InstigatorCharacter->Follower->GetComponentByClass(UxSeedlingStateComponent::StaticClass()));
		if (ensure(SeedlingState))
		{
			SeedlingState->SetOwningPlayer(InstigatorCharacter);
		}
	}

	StopAction(InstigatorCharacter);
}
