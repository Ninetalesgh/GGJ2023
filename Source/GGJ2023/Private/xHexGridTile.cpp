// Fill out your copyright notice in the Description page of Project Settings.


#include "xHexGridTile.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AxHexGridTile::AxHexGridTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AxHexGridTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AxHexGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

