#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckHP.generated.h"

UCLASS()
class ACTIONROGUELIKE_API USBTService_CheckHP : public UBTService
{
	GENERATED_BODY()

private:
	USBTService_CheckHP();

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector NeedHealth;

	UPROPERTY(EditAnywhere, Category = "AI")
	float HealThreshold;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
