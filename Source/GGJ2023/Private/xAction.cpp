// Fill out your copyright notice in the Description page of Project Settings.


#include "xAction.h"
#include "xActionComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

void UxAction::Initialize(UxActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
	ActionAnimDelay = 0.0f;
}

void UxAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, L"Started: %s", *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(L"Started: %s", *ActionName.ToString()), FColor::Green);
	
	if (!InstigatorActor)
	{
		InstigatorActor = GetOwningComponent()->GetOwner();
	}

	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
	RepData = { true, InstigatorActor };

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		Character->PlayAnimMontage(ActionAnimation);
	}

	if (InstigatorActor->HasAuthority())
	{
		ServerOnlyActionPart(InstigatorActor);
	}
}

void UxAction::ServerOnlyActionPart_Implementation(AActor* InstigatorActor)
{
}

void UxAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, L"Stopped: %s", *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(L"Stopped: %s", *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);
	RepData = { false, InstigatorActor };

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool UxAction::CanStart_Implementation(AActor* InstigatorActor)
{
	return !IsRunning() && !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

UWorld* UxAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}


bool UxAction::IsRunning() const
{
	return RepData.bIsRunning;
}

UxActionComponent* UxAction::GetOwningComponent() const
{
	return ActionComp;
}

void UxAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UxAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UxAction, RepData);
	DOREPLIFETIME(UxAction, TimeStarted);
	DOREPLIFETIME(UxAction, ActionComp);
}