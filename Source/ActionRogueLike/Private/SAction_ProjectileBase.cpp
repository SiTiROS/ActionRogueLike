#include "SAction_ProjectileBase.h"
#include "SBaseProjectile.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarDrawDebugProjectile(TEXT("su.ProjectileDebugDraw"),
                                                          false, TEXT("Enable Debug for Projectile. "), ECVF_Cheat);

USAction_ProjectileBase::USAction_ProjectileBase()
	: AttackAnimDelay(0.2f), HandSocketName("Muzzle_01")
{
}

void USAction_ProjectileBase::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (ACharacter* Character = Cast<ACharacter>(Instigator))
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect,
		                                       Character->GetMesh(),
		                                       HandSocketName,
		                                       FVector::ZeroVector,
		                                       FRotator::ZeroRotator,
		                                       EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, TimerDelegate, AttackAnimDelay, false);
	}
}

void USAction_ProjectileBase::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	bool bDrawDebug = CVarDrawDebugProjectile.GetValueOnGameThread();

	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		// Trace shape
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InstigatorCharacter);

		// Type collision 
		FCollisionObjectQueryParams ObjQueryParams;
		ObjQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + InstigatorCharacter->GetControlRotation().Vector() * 5000.0f;

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjQueryParams, Shape, QueryParams))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		//FRotator ProjRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEnd);
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		if (bDrawDebug)
		{
			FColor LineColor = Hit.IsValidBlockingHit() ? FColor::Red : FColor::Green;
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 2.0f, 0, 1.0f);
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.0f, 32, FColor::Red, false, 2.0f);
		}

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
