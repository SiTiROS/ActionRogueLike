#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
	: Health(100.0f), MaxHealth(100.0f)
{
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	if (Health >= MaxHealth && Health <= 0.0f)
	{
		return false;
	}

	if ((Delta + Health) > MaxHealth)
	{
		Health = MaxHealth;
		OnHealthChanged.Broadcast(nullptr, this, Health, Delta); // транслирую изменения в делегат
		return true;
	}

	if ((Delta + Health) < 0.0f)
	{
		Health = 0.0f;
		OnHealthChanged.Broadcast(nullptr, this, Health, Delta); // транслирую изменения в делегат
		return true;
	}

	Health += Delta;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta); // транслирую изменения в делегат
	return true;
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
