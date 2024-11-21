#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign Behavior Tree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if (MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	// }
}
