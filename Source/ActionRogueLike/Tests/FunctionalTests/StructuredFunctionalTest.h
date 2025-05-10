#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "StructuredFunctionalTest.generated.h"

class ATargetPoint;
class ASItemChest;

UCLASS()
class ACTIONROGUELIKE_API AStructuredFunctionalTest : public AFunctionalTest
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = "Setup|FunctionalTest")
	virtual void StartTest() override;

protected:
	// Переопределяем FinishTest для перезагрузки уровня
	virtual void FinishTest(EFunctionalTestResult TestResult, const FString& Message) override;
	
	UPROPERTY(EditInstanceOnly, Category = "Setup|FunctionalTest")
	ASItemChest* ItemChest;

	UPROPERTY(EditInstanceOnly, Category = "Setup|FunctionalTest")
	ATargetPoint* TargetPoint;

	UPROPERTY(EditInstanceOnly, Category = "Setup|FunctionalTest")
	float TimeOut;
};
