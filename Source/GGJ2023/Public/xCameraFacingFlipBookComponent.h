// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "xCameraFacingFlipBookComponent.generated.h"

/**
 * 
 */
UCLASS(ShowCategories = (Mobility, ComponentReplication), ClassGroup = Paper2D, meta = (BlueprintSpawnableComponent))
class GGJ2023_API UxCameraFacingFlipBookComponent : public UPaperFlipbookComponent
{
	GENERATED_BODY()
public:
		UxCameraFacingFlipBookComponent();
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
