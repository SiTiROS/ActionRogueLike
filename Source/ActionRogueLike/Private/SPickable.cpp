#include "SPickable.h"

ASPickable::ASPickable()
	: Cooldown(10.0f), bIsCooldown(false)
{
	PickableMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickableMesh");
	RootComponent = PickableMesh;
}

void ASPickable::BeginPlay()
{
	Super::BeginPlay();
}

void ASPickable::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bIsCooldown)
	{
		bIsCooldown = true;
		PickableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PickableMesh->SetVisibility(false);
		GetWorld()->GetTimerManager().SetTimer(TimerToSpawn, this, &ASPickable::ActivateCooldown, Cooldown);
	}
}

void ASPickable::ActivateCooldown()
{
	PickableMesh->SetVisibility(true);
	PickableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	bIsCooldown = false;
}
