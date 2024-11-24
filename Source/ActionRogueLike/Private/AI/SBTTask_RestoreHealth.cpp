#include "AI/SBTTask_RestoreHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_RestoreHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ASAICharacter* AIPawn = Cast<ASAICharacter>(AIController->GetCharacter());
		if (!AIPawn) return EBTNodeResult::Failed;

		USAttributeComponent* Health = AIPawn->GetComponentByClass<USAttributeComponent>();
		if (Health->ApplyHealthChange(AIPawn, Health->GetMaxHealth()))
		{
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
