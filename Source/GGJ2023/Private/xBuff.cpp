// Fill out your copyright notice in the Description page of Project Settings.


#include "xBuff.h"
#include "xActionComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/Character.h"


UxBuff::UxBuff()
{
	bAutoStart = true;
}

void UxBuff::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UxBuff::StopAction, Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UxBuff::ExecutePeriodicEffect, Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}


void UxBuff::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	if (UxActionComponent* Comp = GetOwningComponent()) Comp->RemoveAction(this);
}

void UxBuff::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	auto* Char = Cast<ACharacter>(Instigator);
	Char->PlayAnimMontage(PeriodicAnimation);
}

float UxBuff::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
}
