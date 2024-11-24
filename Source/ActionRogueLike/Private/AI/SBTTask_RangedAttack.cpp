#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

USBTTask_RangedAttack::USBTTask_RangedAttack()
	: MaxBulletSpread(2.0f)
{
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ACharacter* AIPawn = AIController->GetCharacter(); // ACharacter* AIPawn = Cast<ACharacter>(AIController->GetPawn());
		if (!AIPawn) return EBTNodeResult::Failed;

		FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (!TargetActor) return EBTNodeResult::Failed;

		if (!USAttributeComponent::IsActorAlive(TargetActor)) return EBTNodeResult::Failed;

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation; // вычисляем угол между оружием бота и игроком
		FRotator MuzzleRotation = Direction.Rotation(); // преобразуем в поворот

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread); // отклонение\разброс снарядов
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread); // отклонение\разброс снарядов

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = AIPawn;

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
