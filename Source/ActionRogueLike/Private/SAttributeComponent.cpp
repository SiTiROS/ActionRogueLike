#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
	: Health(100.0f)
{
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta); // транслирую изменения в делегат

	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
