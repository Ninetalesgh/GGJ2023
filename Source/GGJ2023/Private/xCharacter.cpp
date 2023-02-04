// Fill out your copyright notice in the Description page of Project Settings.


#include "xCharacter.h"
#include "xActionComponent.h"
#include "xPlayerController.h"
#include "xCameraFacingFlipBookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "xFactionComponent.h"

AxCharacter::AxCharacter()
{
	ActionComp = CreateDefaultSubobject<UxActionComponent>("ActionComp");
	BodyComp = CreateDefaultSubobject<UxCameraFacingFlipBookComponent>("BodyComp");
	FactionComp = CreateDefaultSubobject<UxFactionComponent>("FactionComp");
	
	BodyComp->SetupAttachment(RootComponent);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AxCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void AxCharacter::MoveUp(float Value)
{
	AddMovementInput(FVector(1, 0, 0), Value);
}

void AxCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector(0,1,0), Value);
}

void AxCharacter::Plant()
{
	ActionComp->StartActionByName(this, "Plant");
}

AxAICharacter* AxCharacter::GetNextSeedling()
{
	return Follower;
}

AxAICharacter* AxCharacter::GetLastSeedling()
{
	return Follower;
}

void AxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveUp", this, &AxCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AxCharacter::MoveRight);

	PlayerInputComponent->BindAction("Plant", IE_Pressed, this, &AxCharacter::Plant);
}

