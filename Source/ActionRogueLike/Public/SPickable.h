#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickable.generated.h"

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASPickable : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	ASPickable();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Setup")
	UStaticMeshComponent* PickableMesh;

	FTimerHandle TimerToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	float Cooldown;
	
	void ShowPowerUp();
	void HideAndCooldownPickup();
	void SetPickupState(bool bNewIsActive);

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
