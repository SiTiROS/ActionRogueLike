#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
	: Health(100.0f)
{
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	return true;
}
