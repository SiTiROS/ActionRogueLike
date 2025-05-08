#include "SBaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"

ASBaseProjectile::ASBaseProjectile()
	: ImpactShakeInnerRadius(250.0f), ImpactShakeOuterRadius(2500.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComp;
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &ASBaseProjectile::OnComponentHit);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true; // Автоматический поворот снаряда в направлении его скорости
	MovementComp->bInitialVelocityInLocalSpace = true; // Начальная скорость снаряда задается в локальной системе координат
	MovementComp->ProjectileGravityScale = 0.0f; // Гравитация 0

	FlightSoundComp = CreateDefaultSubobject<UAudioComponent>("FlightSound");
	FlightSoundComp->SetupAttachment(RootComponent);

	AActor::SetLifeSpan(4.0f);

	// SetReplicates(true);
}

void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASBaseProjectile::OnComponentHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                     FVector NormalImpulse,
                                                     const FHitResult& Hit)
{
	Explode();
}

void ASBaseProjectile::Explode_Implementation()
{
	ensureAlways(IsValid(this)); // чтобы 2 раза не дестроили, где-то в наследниках

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());

	UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	Destroy();
}
