#include "SDashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
	: TeleportDelay(0.2f), ActivateDelay(0.2f)
{
	MovementComp->InitialSpeed = 6000.f;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Activate, this, &ASDashProjectile::Explode, ActivateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Activate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	// FTimerHandle TimerHandle_Teleport;
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
		// ActorToTeleport->TeleportTo(GetActorLocation(), GetActorRotation());
	}
}
