// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/SPickablePowerup.h"
#include "SCharacter.h"
#include "Components/SActionComponent.h"
#include "SAction.h"


void ASPickablePowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	// Make sure we have instigator & that action class was set up
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	USActionComponent* ActionComponent = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
	if (ActionComponent)
	{
		if (ActionComponent->GetAction(ActionToGrant))
		{
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		// Give new Ability
		ActionComponent->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPickup();
	}
}
