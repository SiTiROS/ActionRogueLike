#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

public:
	ASDashProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Teleport")
	float ActivateDelay;

	FTimerHandle TimerHandle_Activate;
	FTimerHandle TimerHandle_Teleport;

	// переопределяем реализацию
	virtual void Explode_Implementation() override;

	virtual void TeleportInstigator();

	virtual void BeginPlay() override;
};
