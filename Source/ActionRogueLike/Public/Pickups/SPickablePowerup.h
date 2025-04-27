// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SPickable.h"
#include "SPickablePowerup.generated.h"

class USAction;

UCLASS()
class ACTIONROGUELIKE_API ASPickablePowerup : public ASPickable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Setup|ActionToGrant")
	TSubclassOf<USAction> ActionToGrant;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
