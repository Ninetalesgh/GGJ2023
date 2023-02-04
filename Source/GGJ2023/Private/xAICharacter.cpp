// Fill out your copyright notice in the Description page of Project Settings.


#include "xAICharacter.h"
#include "xCharacter.h"
#include "xCameraFacingFlipBookComponent.h"
#include "xSeedlingStateComponent.h"
#include "xActionComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/SkeletalMeshComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AxAICharacter::AxAICharacter()
{
	//AttributeComp = CreateDefaultSubobject<UxAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UxActionComponent>("ActionComp");
	SeedlingStateComp = CreateDefaultSubobject<UxSeedlingStateComponent>("SeedlingStateComp");
	HatComp = CreateDefaultSubobject<UBillboardComponent>("HatComp");
	BodyComp = CreateDefaultSubobject<UxCameraFacingFlipBookComponent>("BodyComp");

	BodyComp->SetupAttachment(RootComponent);
	HatComp->SetupAttachment(BodyComp);

	GetMesh()->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AxAICharacter::BeginPlay()
{
	Super::BeginPlay();	
}

AxAICharacter* AxAICharacter::GetPrevious()
{
	return nullptr;
}

ACharacter* AxAICharacter::GetNext()
{
	return nullptr;
}

void AxAICharacter::SetNext(ACharacter* Next)
{

}

void AxAICharacter::SetPrevious(AxAICharacter* Previous)
{

}


