#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
	: Health(100.0f), MaxHealth(100.0f)
{
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth; // показывает то, что реально отнялось или прибавилось
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta); // транслирую изменения в делегат
	return ActualDelta != 0.0f;
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

float USAttributeComponent::GetHealth()
{
	return Health;
}

float USAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health >= MaxHealth;
}
