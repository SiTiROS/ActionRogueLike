#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
	: TimeToHitParamName("TimeToHit")
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), Pawn->GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget) const
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f)
		{
			// Stop BT
			if (const AAIController* AIController = Cast<AAIController>(GetController()))
			{
				AIController->GetBrainComponent()->StopLogic("Killed");
			}

			// Ragdoll
			GetMesh()->SetCollisionProfileName("Ragdoll"); // PhysicsActor
			GetMesh()->SetAllBodiesSimulatePhysics(true);

			// Set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
