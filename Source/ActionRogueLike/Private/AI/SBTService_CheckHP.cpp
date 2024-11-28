#include "AI/SBTService_CheckHP.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHP::USBTService_CheckHP()
	: HealThreshold(0.2f)
{
}

void USBTService_CheckHP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		USAttributeComponent* HealthComp = USAttributeComponent::GetAttributes(AIPawn); // AICharacter->GetComponentByClass<USAttributeComponent>()
		if (ensure(HealthComp))
		{
			bool bNeedHealth = (HealthComp->GetHealth() / HealthComp->GetMaxHealth() <= HealThreshold);

			OwnerComp.GetBlackboardComponent()->SetValueAsBool(NeedHealth.SelectedKeyName, bNeedHealth);
		}
	}
}
