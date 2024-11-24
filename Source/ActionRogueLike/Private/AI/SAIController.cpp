#include "AI/SAIController.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign Behavior Tree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
