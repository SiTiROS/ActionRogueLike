#include "SPickable.h"

ASPickable::ASPickable()
	: Cooldown(10.0f)
{
	PickableMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickableMesh");
	RootComponent = PickableMesh;
}

void ASPickable::Interact_Implementation(APawn* InstigatorPawn)
{
}

void ASPickable::ShowPowerUp()
{
	SetPickupState(true);
}

void ASPickable::HideAndCooldownPickup()
{
	SetPickupState(false);
	GetWorldTimerManager().SetTimer(TimerToSpawn, this, &ASPickable::ShowPowerUp, Cooldown);
}

void ASPickable::SetPickupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	PickableMesh->SetVisibility(bNewIsActive, true);
}
