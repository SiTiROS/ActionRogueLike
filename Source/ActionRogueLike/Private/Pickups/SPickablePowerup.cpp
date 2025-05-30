// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickups/SPickablePowerup.h"
#include "Components/SActionComponent.h"
#include "SAction.h"
#include "SUtils.h"

void ASPickablePowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	// Проверка InstigatorPawn и заполнен ли ActionToGrant
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	if (USActionComponent* ActionComponent = SUtils::GetComponent<USActionComponent>(InstigatorPawn))
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
