#include "Components/SAttributeComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"),
                                                        1.0f, TEXT("Global Damage Modifier for Attribute Component. "), ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
	: Health(100.0f), MaxHealth(Health), Rage(0.0f), MaxRage(100.0f)
{
	SetIsReplicatedByDefault(true);
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

	if (ActualDelta != 0.0f)
	{
		// OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta); // транслирую изменения в делегат
		MulticastHealthChange(InstigatorActor, Health, ActualDelta); // реплицированная передача в делегату		
	}

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

	Rage = FMath::Clamp(Rage + Delta, 0.0f, MaxRage);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		// OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
		MulticastRageChange(InstigatorActor, Rage, ActualDelta);
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

float USAttributeComponent::GetRagePercent() const
{
	return Rage / MaxRage;
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

void USAttributeComponent::MulticastHealthChange_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChange_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
	DOREPLIFETIME(USAttributeComponent, MaxRage);
	DOREPLIFETIME(USAttributeComponent, Rage);


	// DOREPLIFETIME_CONDITION(USAttributeComponent, Health, COND_OwnerOnly); // доступно только владельцу
	// DOREPLIFETIME_CONDITION(USAttributeComponent, Health, COND_InitialOnly); // только при первом появлении
}
