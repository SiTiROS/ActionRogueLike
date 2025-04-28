#include "Pickups/SPickable.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ASPickable::ASPickable()
	: Cooldown(10.0f)
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//SphereComp->SetCollisionProfileName("Pickups");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetupAttachment(RootComponent);

	SetReplicates(true);
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
	RootComponent->SetVisibility(bNewIsActive, true);
}
