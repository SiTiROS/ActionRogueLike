#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USActionComponent;
class USAttributeComponent;
class USInteractionComponent;
class USpringArmComponent;
class UCameraComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup|Component")
	USActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, Category = "Setup|Attack|FX|Material")
	FName TimeToHitParamName;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void SprintStart();

	void SprintStop();

	void PrimaryAttack();

	void SecondaryAttack();

	void Dash();

	UFUNCTION(BlueprintCallable)
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//debug
	UFUNCTION(Exec) // доступна из консоли
	void HealSelf(float Amount = 100);

	UFUNCTION(Exec) // доступна из консоли
	void HealMaxSelf();
};
