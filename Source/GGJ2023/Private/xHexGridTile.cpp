// Fill out your copyright notice in the Description page of Project Settings.


#include "xHexGridTile.h"
#include "xFactionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AxHexGridTile::AxHexGridTile()
{
 	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	FactionComp = CreateDefaultSubobject<UxFactionComponent>("FactionComp");
}

void AxHexGridTile::BeginPlay()
{
	Super::BeginPlay();	
}
