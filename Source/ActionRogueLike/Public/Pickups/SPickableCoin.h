#pragma once

#include "CoreMinimal.h"
#include "SPickable.h"
#include "SPickableCoin.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPickableCoin : public ASPickable
{
	GENERATED_BODY()

public:
	ASPickableCoin();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Credits")
	float CreditsAmount;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
