#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileBase.generated.h"

class UAnimMontage;
class UParticleSystem;
class ASBaseProjectile;

UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileBase : public USAction
{
	GENERATED_BODY()

public:
	USAction_ProjectileBase();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Setup|Attack")
	TSubclassOf<ASBaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack|FX")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack|FX")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack")
	UAnimMontage* AttackAnim;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
};
