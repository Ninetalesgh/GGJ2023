// Fill out your copyright notice in the Description page of Project Settings.


#include "xCameraFacingFlipBookComponent.h"
#include "xCharacter.h"
#include "xPlayerController.h"
#include "xAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "../GGJ2023.h"

UxCameraFacingFlipBookComponent::UxCameraFacingFlipBookComponent()
{
	SetComponentTickEnabled(true);	
}

void UxCameraFacingFlipBookComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* Char = Cast<ACharacter>(GetOwner());
	auto* Player = GEngine->GetFirstLocalPlayerController(GetWorld());
		
	ULocalPlayer* const LP = Player ? Player->GetLocalPlayer() : nullptr;
	if (LP && LP->ViewportClient)
	{
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
		{
			FColor LineColor = FColor::Green;
			DrawDebugLine(GetWorld(), ProjectionData.ViewOrigin, Char->GetActorLocation(), LineColor, false, 0.0f, 0, 2.0f);
			
			FVector Rot = Char->GetActorLocation() - ProjectionData.ViewOrigin;
			Rot.Z = 0.0f;

			FRotator Rotor = UKismetMathLibrary::MakeRotFromX(Rot);
			Rotor.Yaw = 90 + Rotor.Yaw;

			SetWorldRotation(FQuat::MakeFromRotator(Rotor));
		}
	}
}
