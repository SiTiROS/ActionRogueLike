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
	
	bool bIsCooldown;

	void ActivateCooldown();
	
	virtual void BeginPlay() override;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};