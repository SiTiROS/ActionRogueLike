#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickable.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASPickable : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	ASPickable();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComp;

	FTimerHandle TimerToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	float Cooldown;

	void ShowPowerUp();
	void HideAndCooldownPickup();
	void SetPickupState(bool bNewIsActive);

	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
