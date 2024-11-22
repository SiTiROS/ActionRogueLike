#include "AI/SBTService_CheckHP.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckHP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ASAICharacter* AICharacter = Cast<ASAICharacter>(AIController->GetPawn());
		if (ensure(AICharacter))
		{
			USAttributeComponent* Health = AICharacter->GetComponentByClass<USAttributeComponent>();
			
			bool bNeedHealth = (Health->GetHealth() / Health->GetMaxHealth() <= HealThreshold);

			OwnerComp.GetBlackboardComponent()->SetValueAsBool(NeedHealth.SelectedKeyName, bNeedHealth);
		}
	}
}
