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

USAttributeComponent* USAttributeComponent::GetAttributes(const AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(const AActor* Actor)
{
	if (const USAttributeComponent* AttributesComp = GetAttributes(Actor))
	{
		return AttributesComp->IsAlive();
	}
	return false;
}