#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class UUserWidget;
class USWorldUserWidget;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, Category = "Setup|FX|Material")
	FName TimeToHitParamName;

	UPROPERTY(EditAnywhere, Category = "Setup|Widgets")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	USWorldUserWidget* ActiveHealthBar;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	void SetTargetActor(AActor* NewTarget) const;
};
