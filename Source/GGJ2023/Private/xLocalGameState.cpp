// Fill out your copyright notice in the Description page of Project Settings.


#include "xLocalGameState.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "xGameMode.h"
#include "Engine/World.h"
#include "../GGJ2023.h"
#include "Kismet/GameplayStatics.h"
#include "xPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"

AxLocalGameState::AxLocalGameState()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");

	RootComponent = SceneComp;
	CameraComp->SetupAttachment(SceneComp);
}

void AxLocalGameState::BeginPlay()
{
	Super::BeginPlay();
	TArray<FInputDeviceId> ConnectedDevices;
	
	IPlatformInputDeviceMapper::Get().GetAllConnectedInputDevices(ConnectedDevices);

	for (auto id : ConnectedDevices)
	{
		if (!HasAuthority())
		{
			LogOnScreen(this, FString::Printf(L"Connected Device -> ID: %d", id.GetId()), FColor::White);

			ULocalPlayer* LocalPlayer = GetWorld()->GetGameInstance()->FindLocalPlayerFromPlatformUserId(FGenericPlatformMisc::GetPlatformUserForUserIndex(id.GetId()));

			if (LocalPlayer == nullptr)
			{
				APlayerController* NewPlayer = UGameplayStatics::CreatePlayer(this, id.GetId(), true);
			}
		}
	}

	InputDeviceChangedDelegate = IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().AddLambda([this](EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId)
		{
			if (!HasAuthority())
			{
				FString ConnectState = NewConnectionState == EInputDeviceConnectionState::Connected ? "Connected" :
					NewConnectionState == EInputDeviceConnectionState::Disconnected ? "Disconnected" : "Invalid";

				ULocalPlayer* LocalPlayer = GetWorld()->GetGameInstance()->FindLocalPlayerFromPlatformUserId(PlatformUserId);

				if (LocalPlayer == nullptr)
				{
					APlayerController* NewPlayer = UGameplayStatics::CreatePlayer(this, PlatformUserId.GetInternalId(), true);
				}

				LogOnScreen(this, FString::Printf(L"%s Device -> ID: %d", *ConnectState, InputDeviceId.GetId()), FColor::White);
			}
		});
}

void AxLocalGameState::BeginDestroy()
{
	IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().Remove(InputDeviceChangedDelegate);
	Super::BeginDestroy();
}
