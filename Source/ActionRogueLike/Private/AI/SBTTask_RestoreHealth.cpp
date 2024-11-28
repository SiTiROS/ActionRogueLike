#include "AI/SBTTask_RestoreHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_RestoreHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* AIPawn = OwnerComp.GetAIOwner()->GetCharacter();
	if (!AIPawn) return EBTNodeResult::Failed;

	USAttributeComponent* HealthComp = USAttributeComponent::GetAttributes(AIPawn);
	if (ensure(HealthComp) && HealthComp->ApplyHealthChange(AIPawn, HealthComp->GetMaxHealth())) return EBTNodeResult::Succeeded;
	
	return EBTNodeResult::Failed;
}
