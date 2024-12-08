#include "SMagicProjectile.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SActionEffect.h"

ASMagicProjectile::ASMagicProjectile()
	: DamageAmount(20.0f)
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		// static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying"); // жёстко закодирован и лучше сделать static чтобы память 1 раз выделилась

		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity; // инвертируем движение снаряда

			SetInstigator(Cast<APawn>(OtherActor)); // задаём нового владельца снаряда, чтобы не игнорировался оверлап
			return;
		}

		// ApplyDamage & Impulse
		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			if (ActionComp && BurningActionClass)
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}
