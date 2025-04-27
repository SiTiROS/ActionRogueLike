#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


class USWorldUserWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USInteractionComponent();

	void PrimaryInteract();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	// Reliable - Will always arrive, eventually. Request will be re-sent unless an acknowledgment was received. 
	// Unreliable - Not guaranteed, packet can get lost and won't retry.
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
	void FindBestInteractable();

	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
};
