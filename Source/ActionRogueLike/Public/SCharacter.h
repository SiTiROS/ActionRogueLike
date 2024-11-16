#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class ASBaseProjectile;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Setup|Component")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup|Component")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Setup|Component")
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup|Component")
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack")
	TSubclassOf<ASBaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack")
	TSubclassOf<ASBaseProjectile> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack")
	TSubclassOf<ASBaseProjectile> DashProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SecondaryAttack;
	FTimerHandle TimerHandle_Dash;

	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void SpawnProjectile(TSubclassOf<ASBaseProjectile> ClassToSpawn);
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void SecondaryAttack();
	void SecondaryAttack_TimeElapsed();
	void Dash();
	void Dash_TimeElapsed();
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
