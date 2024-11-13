#include "SExplosiveBarrel.h"
#include "SMagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName("PhysicsActor");

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 400.0f;
	RadialForceComp->bImpulseVelChange = true; // игнорировать массу
	RadialForceComp->ImpulseStrength = 1000.0f;
	RadialForceComp->SetAutoActivate(false);
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComp->SetupAttachment(MeshComp);
	EffectComp->bAutoActivate = false;
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Лучше тут вызывать привязку
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnComponentHit);
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	EffectComp->Deactivate();

	// MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnComponentHit);
}

void ASExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                       FVector NormalImpulse, const FHitResult& Hit)
{
	ASMagicProjectile* ProjActor = Cast<ASMagicProjectile>(OtherActor);
	if (ProjActor)
	{
		RadialForceComp->FireImpulse();
		Destroy();
	}
}

void ASExplosiveBarrel::Destroyed()
{
	Super::Destroyed();
	if (EffectComp)
	{
		// Получаем текущую позицию и ориентацию
		FVector Location = GetActorLocation();
		FRotator Rotation = GetActorRotation();

		// Спавним эффект
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EffectComp->Template, Location, Rotation);
	}
}
