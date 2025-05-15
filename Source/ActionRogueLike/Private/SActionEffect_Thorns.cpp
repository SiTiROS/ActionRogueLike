// Fill out your copyright notice in the Description page of Project Settings.

#include "SActionEffect_Thorns.h"
#include "Components/SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SActionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


USActionEffect_Thorns::USActionEffect_Thorns()
	: ReflectFraction(0.2f)
{
	bAutoStart = false;
	Duration = 0.0f;
	Period = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// UE_LOG(LogTemp, Error, TEXT("StartThorns"));

	// Start FX
	FX = UGameplayStatics::SpawnEmitterAttached(ThornsEffect,
	                                            GetOwningComponent()->GetOwner()->GetRootComponent(),
	                                            NAME_None,
	                                            FVector::ZeroVector,
	                                            FRotator::ZeroRotator,
	                                            EAttachLocation::SnapToTarget,
	                                            true,
	                                            EPSCPoolMethod::None,
	                                            true
	);

	// Start listening
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop FX
	if (FX)
	{
		FX->DeactivateSystem();
		FX->DestroyComponent();
	}

	// Stop listening
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	if (OwningActor != InstigatorActor && Delta <= 0.0f)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0) return;

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// Return damage sender...
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}