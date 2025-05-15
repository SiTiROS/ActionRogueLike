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

	/* Material parameter for Hitflashes */
	UPROPERTY(EditAnywhere, Category = "Setup|FX|Material")
	FName TimeToHitParamName;

	UPROPERTY(EditAnywhere, Category = "Setup|Widgets")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	USWorldUserWidget* ActiveHealthBar;

	/* Widget to display when bot first sees a player. */
	UPROPERTY(EditDefaultsOnly, Category = "Setup|UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Setup|AI")
	AActor* GetTargetActor() const;

	UFUNCTION(BlueprintCallable, Category = "Setup|AI")
	void SetTargetActor(AActor* NewTarget) const;

	/* Key for AI Blackboard 'TargetActor' */
	UPROPERTY(VisibleAnywhere, Category = "Setup|AI")
	FName TargetActorKey;

	UFUNCTION(NetMulticast, Unreliable) 
	void Multicast_ShowWidget();
};
