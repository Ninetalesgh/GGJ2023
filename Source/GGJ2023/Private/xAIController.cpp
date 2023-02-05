// Fill out your copyright notice in the Description page of Project Settings.


#include "xAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AxAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, L"Behaviour Tree is nullptr!"))
	{
		RunBehaviorTree(BehaviorTree);
	}	
}

void AxAIController::SetTarget(AActor* Target)
{
	if (Target)
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", Target);
	}
}
