// Fill out your copyright notice in the Description page of Project Settings.


#include "xLocalGameState.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "xGameMode.h"
#include "Engine/World.h"

void AxLocalGameState::BeginPlay()
{
	Super::BeginPlay();
	TArray<FInputDeviceId> Devices;
	
	IPlatformInputDeviceMapper::Get().GetAllConnectedInputDevices(Devices);

	IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().AddLambda([this](EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId)
		{	
			int test = 0;
			auto* GM = GetWorld()->GetAuthGameMode<AxGameMode>();

			UE_LOG(LogTemp, Log, L"input devices: ");
		
		});

	UE_LOG(LogTemp, Log, L"input devices: ");

}

