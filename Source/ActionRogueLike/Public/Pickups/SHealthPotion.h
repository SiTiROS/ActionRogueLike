#pragma once

#include "CoreMinimal.h"
#include "SPickable.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickable
{
	GENERATED_BODY()

public:
	ASHealthPotion();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Heal")
	float HealthToHeal;
	
	UPROPERTY(EditAnywhere, Category = "Setup|Credit")
	int32 CreditCost;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
