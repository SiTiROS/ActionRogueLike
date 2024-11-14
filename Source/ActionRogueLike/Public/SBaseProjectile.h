#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASBaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	ASBaseProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Components")
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Effects")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Effects")
	UParticleSystem* ImpactVFX;

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                            const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void BeginPlay() override;;
};
