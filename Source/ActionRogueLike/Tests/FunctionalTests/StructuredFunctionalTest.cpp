#include "StructuredFunctionalTest.h"
#include "SCharacter.h"
#include "SItemChest.h"
#include "Components/SInteractionComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void AStructuredFunctionalTest::StartTest()
{
	Super::StartTest();


	SetTimeLimit(TimeOut, EFunctionalTestResult::Failed);

	ASCharacter* Character = Cast<ASCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!ensure(Character))
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Character is none"));
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!ensure(PlayerController))
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("PlayerController is none"));
		return;
	}

	Character->DisableInput(PlayerController);

	if (!ensure(TargetPoint))
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("TargetPoint is none"));
		return;
	}

	if (!ensure(ItemChest))
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("ItemChest is none"));
		return;
	}

	// Тпшим перса и поворачиваем
	FVector CharacterLocation = Character->GetActorLocation();
	FVector TargetLocation = TargetPoint->GetActorLocation();
	FVector ItemChestLocation = ItemChest->GetActorLocation();

	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, ItemChestLocation);

	FTransform Transform(LookRotation, TargetLocation, FVector(1.f, 1.f, 1.f));
	Character->SetActorTransform(Transform);

	// Поворачиваем камеру перса
	FVector EyesLocation;
	FRotator EyesRotation;
	Character->GetActorEyesViewPoint(EyesLocation, EyesRotation);

	USpringArmComponent* SpringArm = Character->FindComponentByClass<USpringArmComponent>();
	if (SpringArm)
	{
		SpringArm->SocketOffset = FVector::ZeroVector; // убираем оффсет, а то с ним криво
		SpringArm->TargetOffset = FVector(0, 0, -100); // опускаем камеру, а то криво
	}

	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(EyesLocation, ItemChestLocation);
	PlayerController->SetControlRotation(DesiredRotation);

	// Взаимодействуем
	USInteractionComponent* InteractionComp = Character->FindComponentByClass<USInteractionComponent>();
	if (InteractionComp)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this, InteractionComp]()
		{
			InteractionComp->PrimaryInteract();

			// Проверяем
			if (ItemChest->IsLidOpened())
			{
				FinishTest(EFunctionalTestResult::Succeeded, TEXT("Chest is opened"));
			}
			else
			{
				FinishTest(EFunctionalTestResult::Failed, TEXT("Chest isn't opened"));
			}
		}, 0.2f, false);
	}
}

void AStructuredFunctionalTest::FinishTest(EFunctionalTestResult TestResult, const FString& Message)
{
	Super::FinishTest(TestResult, Message);
	
	// Получаем имя текущего уровня и перезагружаем его
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}
