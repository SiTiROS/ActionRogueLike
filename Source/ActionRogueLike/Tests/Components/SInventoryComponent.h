#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/STypes.h"
#include "SInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API USInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USInventoryComponent();

	bool TryToAddItem(const FInventoryData& Data);

	UFUNCTION(BlueprintCallable)
	int32 GetInventoryAmountByType(EInventoryItemType Type) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup|Inventory")
	TMap<EInventoryItemType, int32> InventoryLimits;

private:
	TMap<EInventoryItemType, int32> Inventory;
};
