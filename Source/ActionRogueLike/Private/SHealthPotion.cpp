#include "SHealthPotion.h"
#include "SCharacter.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
	: HealthToHeal(50.0f)
{
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ASCharacter* Character = Cast<ASCharacter>(InstigatorPawn))
	{
		USAttributeComponent* Health = Cast<USAttributeComponent>(Character->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (ensure(Health) && !Health->IsFullHealth() && Health->IsAlive())
		{
			if (Health->ApplyHealthChange(HealthToHeal))
			{
				HideAndCooldownPickup();
				UE_LOG(LogTemp, Error, TEXT("ApplyHealthChange"));
			}
		}
	}
}
