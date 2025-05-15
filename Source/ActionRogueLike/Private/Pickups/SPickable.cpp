#include "Pickups/SPickable.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ASPickable::ASPickable()
	: Cooldown(10.0f), bIsActive(true)
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
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void ASPickable::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void ASPickable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickable, bIsActive);
}
