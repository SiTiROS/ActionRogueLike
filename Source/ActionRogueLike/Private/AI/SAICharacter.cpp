#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/SActionComponent.h"
#include "Components/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Widgets/SWorldUserWidget.h"

ASAICharacter::ASAICharacter()
	: TimeToHitParamName("TimeToHit")
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));

	// Ensures we receive a controlled when spawned in the level by our game mode
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Disabled on capsule to let projectiles pass through capsule and hit mesh instead
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);

	// Enabled on mesh to react to incoming projectiles
	GetMesh()->SetGenerateOverlapEvents(true);

	TargetActorKey = "TargetActor";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	// Ignore if target already set
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);

		// Добавляем виджет ! знак
		USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (NewWidget)
		{
			NewWidget->AttachedActor = this;
			// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
			// May end up behind the minion health bar otherwise.
			NewWidget->AddToViewport(10);
		}
	}


	//DrawDebugString(GetWorld(), Pawn->GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
}

AActor* ASAICharacter::GetTargetActor() const
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
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

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Died
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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// Set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
