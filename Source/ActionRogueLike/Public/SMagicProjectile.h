#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SBaseProjectile.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

public:
	ASMagicProjectile();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Setup|Damage")
	float DamageAmount;

	UPROPERTY(EditAnywhere, Category = "Setup|Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditAnywhere, Category = "Setup|Damage")
	TSubclassOf<USActionEffect> BurningActionClass;
};
