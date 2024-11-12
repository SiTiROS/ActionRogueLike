#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ASMagicProjectile::ASMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComp;
	// Устанавливаем тип профиля коллизии
	SphereComp->SetCollisionProfileName("Projectile");

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComp->SetupAttachment(SphereComp);

	// Компонент для управления движением снаряда
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	// Начальная скорость снаряда в 1000 единиц в секунду
	MovementComp->InitialSpeed = 1000.0f;
	// Автоматический поворот снаряда в направлении его скорости
	MovementComp->bRotationFollowsVelocity = true;
	// Начальная скорость снаряда задается в локальной системе координат
	MovementComp->bInitialVelocityInLocalSpace = true;
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
