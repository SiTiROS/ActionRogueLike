#pragma once

#include "CoreMinimal.h"
#include "STypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
	SPHERE = 0 UMETA(DisplayName = "MY SPHERE"),
	CUBE UMETA(DisplayName = "MY CUBE"),
	CYLINDER,
	CONE
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 Score;
};
