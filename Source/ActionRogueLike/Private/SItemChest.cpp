#include "SItemChest.h"

ASItemChest::ASItemChest()
	: TargetPitch{110.0f}
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	//ISGameplayInterface::Interact_Implementation(InstigatorPawn);

	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}

void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
}

void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
