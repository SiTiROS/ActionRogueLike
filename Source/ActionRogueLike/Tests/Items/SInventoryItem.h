#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/STypes.h"
#include "SInventoryItem.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASInventoryItem : public AActor
{
	GENERATED_BODY()

public:
	ASInventoryItem();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Setup|InventoryItem")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|InventoryItem")
	FInventoryData InventoryData;
};
