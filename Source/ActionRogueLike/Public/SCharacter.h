#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack();
	void PrimaryInteract();
	void PrimaryAttack_TimeElapsed();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
