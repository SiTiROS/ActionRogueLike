#include "SHealthPotion.h"
#include "SCharacter.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
	: HealthToHeal(50.0f)
{
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	ASCharacter* Character = Cast<ASCharacter>(InstigatorPawn);

	if (Character)
	{
		USAttributeComponent* Health = Cast<USAttributeComponent>(Character->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (Health && Health->GetHealth() >= Health->GetMaxHealth()) return;

		if (Health->GetHealth() > 0 && Health->GetHealth() < Health->GetMaxHealth() && Health->IsAlive())
		{
			Health->ApplyHealthChange(HealthToHeal);
		}
	}

	Super::Interact_Implementation(InstigatorPawn);
}
