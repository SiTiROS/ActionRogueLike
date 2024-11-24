#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		if (USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())))
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -20.0f);

			Explode();
		}
	}
}
