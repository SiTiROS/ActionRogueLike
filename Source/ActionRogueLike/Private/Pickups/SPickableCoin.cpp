#include "Pickups/SPickableCoin.h"
#include "SPlayerState.h"

ASPickableCoin::ASPickableCoin()
	: CreditsAmount(1)
{
}

void ASPickableCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!InstigatorPawn) return;

	if (ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PlayerState->AddCredits(CreditsAmount);

		HideAndCooldownPickup();
		UE_LOG(LogTemp, Error, TEXT("AddCredits: %f"), CreditsAmount);
	}
}