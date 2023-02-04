// Fill out your copyright notice in the Description page of Project Settings.


#include "xCameraFacingFlipBookComponent.h"
#include "xCharacter.h"
#include "xPlayerController.h"
#include "xLocalGameState.h"
#include "xAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "../GGJ2023.h"

UxCameraFacingFlipBookComponent::UxCameraFacingFlipBookComponent()
{
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
	SetComponentTickEnabled(true);	
}

void UxCameraFacingFlipBookComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* Char = Cast<ACharacter>(GetOwner());
		
	auto* GS = Cast<AxLocalGameState>(GetWorld()->GetGameState());
	if (GS)
	{
		FVector CamLoc = GS->MainCameraLocation;
		FVector CamForward = GS->MainCameraForward;

		FVector Rot = Char->GetActorLocation() - CamLoc;

		FRotator Rotor = UKismetMathLibrary::MakeRotFromX(-CamForward);
		Rotor.Roll = Rotor.Pitch;
		Rotor.Pitch = 0;
		Rotor.Yaw = 90 + Rotor.Yaw;

		SetWorldRotation(FQuat::MakeFromRotator(Rotor));

	}
}
