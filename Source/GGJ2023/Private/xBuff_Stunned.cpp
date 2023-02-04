// Fill out your copyright notice in the Description page of Project Settings.


#include "xBuff_Stunned.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

UxBuff_Stunned::UxBuff_Stunned()
{
	Duration = 0.5f;
}

void UxBuff_Stunned::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	APlayerController* PC = Cast<APlayerController>(Character->GetController());

	Character->DisableInput(PC);
}

void UxBuff_Stunned::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	APlayerController* PC = Cast<APlayerController>(Character->GetController());

	Character->EnableInput(PC);
}