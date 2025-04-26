#include "Components/SAttributeComponent.h"
#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"),
                                                        1.0f, TEXT("Global Damage Modifier for Attribute Component. "), ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
	: Health(100.0f), MaxHealth(100.0f), Rage(0.0f), RageMax(100.0f)
{
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) // движковая функцию, для чита god
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float ActualDelta = Health - OldHealth; // показывает то, что реально отнялось или прибавилось
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta); // транслирую изменения в делегат

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor); // в гейм моде подсчёт очков
		}
	}

	return ActualDelta != 0.0f;
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

bool USAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}

	return ActualDelta != 0;
}


float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float USAttributeComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health >= MaxHealth;
}

void USAttributeComponent::HealMaxHP()
{
	ApplyHealthChange(GetOwner(), GetMaxHealth());
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
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
