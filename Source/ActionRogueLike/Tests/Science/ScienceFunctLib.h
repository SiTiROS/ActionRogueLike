#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFunctLib.generated.h"


UCLASS()
class ACTIONROGUELIKE_API UScienceFunctLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintPure, Category = "Science")
	static int32 Fibonacci(int32 Value);
};