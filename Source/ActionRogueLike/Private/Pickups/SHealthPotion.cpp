#include "Pickups/SHealthPotion.h"
#include "SCharacter.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASHealthPotion::ASHealthPotion()
	: HealthToHeal(50.0f), CreditCost(1)
{
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ASCharacter* Character = Cast<ASCharacter>(InstigatorPawn))
	{
		USAttributeComponent* Health = USAttributeComponent::GetAttributes(InstigatorPawn);

		if (ensure(Health) && !Health->IsFullHealth() && Health->IsAlive())
		{
			if (ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>())
			{
				if (PlayerState->RemoveCredits(CreditCost) && Health->ApplyHealthChange(this, HealthToHeal))
				{
					HideAndCooldownPickup();
					UE_LOG(LogTemp, Error, TEXT("ApplyHealthChange"));
				}
			}
		}
	}
}
