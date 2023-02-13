// Fill out your copyright notice in the Description page of Project Settings.


#include "xLocalGameState.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "xGameMode.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "../GGJ2023.h"
#include "Kismet/GameplayStatics.h"
#include "xPlayerController.h"
#include "Camera/CameraActor.h"


AxLocalGameState::AxLocalGameState()
{
}

void AxLocalGameState::BeginPlay()
{
	Super::BeginPlay();
	TArray<FInputDeviceId> ConnectedDevices;
	
	IPlatformInputDeviceMapper::Get().GetAllConnectedInputDevices(ConnectedDevices);

	//DEFAULT POOP
	{
		TArray<AActor*> Cameras;
		UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), Cameras);

		if (!Cameras.IsEmpty())
		{
			MainCameraLocation = Cameras[0]->GetActorLocation();
			MainCameraForward = Cameras[0]->GetActorForwardVector();

			ULocalPlayer* LocalPlayer = GetWorld()->GetGameInstance()->FindLocalPlayerFromPlatformUserId(FGenericPlatformMisc::GetPlatformUserForUserIndex(0));
			if (auto* PC = LocalPlayer->GetPlayerController(GetWorld()))
			{
				LocalPlayer->GetPlayerController(GetWorld())->SetViewTarget(Cameras[0]);
			}
		}
	}

	for (auto id : ConnectedDevices)
	{
		LogOnScreen(this, FString::Printf(L"Connected Device -> ID: %d", id.GetId()), FColor::White);

		ULocalPlayer* LocalPlayer = GetWorld()->GetGameInstance()->FindLocalPlayerFromPlatformUserId(FGenericPlatformMisc::GetPlatformUserForUserIndex(id.GetId()));

		if (LocalPlayer == nullptr)
		{
			APlayerController* NewPlayer = UGameplayStatics::CreatePlayer(this, id.GetId(), true);
		}
	}

	InputDeviceChangedDelegate = IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().AddLambda([this](EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId)
		{
			FString ConnectState = NewConnectionState == EInputDeviceConnectionState::Connected ? "Connected" :
				NewConnectionState == EInputDeviceConnectionState::Disconnected ? "Disconnected" : "Invalid";

			ULocalPlayer* LocalPlayer = GetWorld()->GetGameInstance()->FindLocalPlayerFromPlatformUserId(PlatformUserId);

			if (LocalPlayer == nullptr)
			{
				APlayerController* NewPlayer = UGameplayStatics::CreatePlayer(this, PlatformUserId.GetInternalId(), true);
			}

			LogOnScreen(this, FString::Printf(L"%s Device -> ID: %d", *ConnectState, InputDeviceId.GetId()), FColor::White);	
		});
}

void AxLocalGameState::BeginDestroy()
{
	IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().Remove(InputDeviceChangedDelegate);
	Super::BeginDestroy();
}
