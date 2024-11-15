#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USAttributeComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes")
	float MaxHealth;
	
	// HealthMax, Stamina, Strength

public:
	UPROPERTY(BlueprintAssignable) // можно подписаться в bp
	FOnHealthChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);
};
