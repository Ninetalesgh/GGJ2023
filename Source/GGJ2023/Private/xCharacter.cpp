// Fill out your copyright notice in the Description page of Project Settings.


#include "xCharacter.h"
#include "xActionComponent.h"
#include "xPlayerController.h"
#include "xAICharacter.h"
#include "xGameMode.h"
#include "xCameraFacingFlipBookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "xFactionComponent.h"
#include "xInteractionComponent.h"

AxCharacter::AxCharacter()
{
	ActionComp = CreateDefaultSubobject<UxActionComponent>("ActionComp");
	BodyComp = CreateDefaultSubobject<UxCameraFacingFlipBookComponent>("BodyComp");
	FactionComp = CreateDefaultSubobject<UxFactionComponent>("FactionComp");
	InteractionComp = CreateDefaultSubobject<UxInteractionComponent>("InteractionComp");
	
	BodyComp->SetupAttachment(RootComponent);
	RootRadius = 3.0f;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AxCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void AxCharacter::MoveUp(float Value)
{
	AddMovementInput(FVector(0, 1, 0), -Value);
}

void AxCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector(1,0,0), Value);
}

void AxCharacter::Plant()
{
	ActionComp->StartActionByName(this, "Plant");
}

void AxCharacter::Uproot()
{
	InteractionComp->Interact();
}

void AxCharacter::MakeSnake()
{
	if (!HasAuthority())
	{
		return;
	}
	auto* GM = Cast<AxGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		auto UprootedSeedlings = GM->GetAllUprootedSeedlingsOfFaction(FactionComp->GetFaction());
		NextFollower = UprootedSeedlings.IsEmpty() ? nullptr : UprootedSeedlings[0];
		if (NextFollower)
		{
			NextFollower->SetNext(this);
			NextFollower->SetOwner(this);

			for (int i = 1; i < UprootedSeedlings.Num(); ++i)
			{
				UprootedSeedlings[i]->SetNext(UprootedSeedlings[i - 1]);
				UprootedSeedlings[i]->SetOwner(this);
			}

			LastFollower = UprootedSeedlings.Last(0);
		}
	}
}


AxAICharacter* AxCharacter::GetNextSeedling()
{
	return NextFollower;
}

AxAICharacter* AxCharacter::GetLastSeedling()
{
	return LastFollower;
}

void AxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveUp", this, &AxCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AxCharacter::MoveRight);

	PlayerInputComponent->BindAction("Plant", IE_Pressed, this, &AxCharacter::Plant);
	PlayerInputComponent->BindAction("Uproot", IE_Pressed, this, &AxCharacter::Uproot);
}

