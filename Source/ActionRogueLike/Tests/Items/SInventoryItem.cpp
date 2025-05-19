#include "Tests/Items/SInventoryItem.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Tests/Components/SInventoryComponent.h"

ASInventoryItem::ASInventoryItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	check(CollisionComponent);
	CollisionComponent->InitSphereRadius(30.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionComponent);
}

void ASInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (const APawn* Pawn = Cast<APawn>(OtherActor))
	{
		if (USInventoryComponent* InventoryComponent = Pawn->FindComponentByClass<USInventoryComponent>())
		{
			if (InventoryComponent->TryToAddItem(InventoryData))
				Destroy();
		}
	}
}
