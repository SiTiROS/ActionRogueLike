#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
	: Credits(0)
{
	bReplicates = true;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f)) return;

	Credits += Delta;
	
	OnCreditsChanged.Broadcast(this, Credits, Delta); // только на сервере
	//MulticastCredits(Credits, Delta);
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f)) return false;

	if (Credits < Delta) return false;

	Credits -= Delta;
	
	OnCreditsChanged.Broadcast(this, Credits, -Delta); // только на сервере
	//MulticastCredits(Credits, -Delta);
	
	return true;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits); // только на клиенте почему то????
	//MulticastCredits(Credits, Credits - OldCredits);
}

void ASPlayerState::MulticastCredits_Implementation(float NewCredits, float Delta)
{
	OnCreditsChanged.Broadcast(this, Credits, Delta); // вообще тут не надо
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}
