#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class UAudioComponent;
class CameraShakeBase;

UCLASS(ABSTRACT) // 'ABSTRACT' marks this class as incomplete, keeping this out of certain dropdowns windows like SpawnActor in Unreal Editor
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup|Effects|VFX")
	UParticleSystem* ImpactVFX;

	UPROPERTY(VisibleAnywhere, Category = "Setup|Effects|Sound")
	UAudioComponent* FlightSoundComp;

	UPROPERTY(EditAnywhere, Category = "Setup|Effects|Sound")
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Effects|Shake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Effects|Shake")
	float ImpactShakeOuterRadius;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                            const FHitResult& Hit);

	// BlueprintNativeEvent = C++ base implementation, can be expanded in Blueprints
	// BlueprintCallable to allow child classes to trigger explosions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void BeginPlay() override;;
};
