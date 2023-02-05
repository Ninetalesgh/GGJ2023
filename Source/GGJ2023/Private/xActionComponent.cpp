// Fill out your copyright notice in the Description page of Project Settings.


#include "xActionComponent.h"
#include "xAction.h"

#include "Engine/ActorChannel.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

UxActionComponent::UxActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UxActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UxAction> ActionClass : DefaultActions)
		{
			if (ActionClass)
			{
				AddAction(GetOwner(), ActionClass);
			}
		}
	}
}

void UxActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<UxAction*> ActionsCopy = Actions;
	for (UxAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UxActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	//DrawAllActions
	for (UxAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(L"[%s] Action: %s", *GetNameSafe(GetOwner()), *GetNameSafe(Action));

		//LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}


UxAction* UxActionComponent::GetAction(TSubclassOf<UxAction> ActionClass) const
{
	for (UxAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}


void UxActionComponent::AddAction(AActor* Instigator, TSubclassOf<UxAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, L"Client attempting to AddAction. [Class: %s]", *GetNameSafe(ActionClass));
		return;
	}

	UxAction* NewAction = NewObject<UxAction>(GetOwner(), ActionClass);

	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UxActionComponent::RemoveAction(UxAction* Action)
{
	if (!ensure(Action && !Action->IsRunning()))
	{
		return;
	}

	Actions.Remove(Action);
}

bool UxActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UxAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(L"Failed to run: %s", *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			TRACE_BOOKMARK(L"Start Action: %s", *GetNameSafe(Action));

			Action->StartAction(Instigator);

			return true;
		}
	}

	return false;
}

bool UxActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UxAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void UxActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UxActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void UxActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UxActionComponent, Actions);
}

bool UxActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UxAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

