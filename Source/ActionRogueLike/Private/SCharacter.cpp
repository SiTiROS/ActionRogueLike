#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SBaseProjectile.h"
#include "Kismet/GameplayStatics.h"

ASCharacter::ASCharacter()
	: AttackAnimDelay(0.2f), HandSocketName("Muzzle_01"), TimeToHitParamName("TimeToHit")
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true; // Настройка, чтобы вращать пружинной палкой с камерой по оси Y

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	bUseControllerRotationYaw = false; // Отвязываем вращение персонажа мышкой
	GetCharacterMovement()->bOrientRotationToMovement = true; // Ориентируем поворот персонажа в сторону движения 

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComp"));
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged); // привязка функции к делегату
}

FVector ASCharacter::GetPawnViewLocation() const
// переопределяем базовую функцию, чтобы скорректировать для нашего вида от 3его лица для GetActorEyesViewPoint версии для pawn
{
	return CameraComp->GetComponentLocation();
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	constexpr float DrawScale = 100.0f;
	constexpr float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();

	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;

	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);

	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);

	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Движение игрока верёд, назад
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	// Движение игрока влево, вправо
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// Движение мышкой вверх, вниз
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	// Движение мышкой влево, вправо
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Атака ЛКМ
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);

	// Прыжок
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	// Взаимодействие
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	// Атака ПКМ
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);

	// Телепорт
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	// Бег
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
}

void ASCharacter::MoveForward(float Value)
{
	// Получить вращение камеры без наклонов
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	// Добавить движение вперёд/назад
	AddMovementInput(ControlRotation.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	// Получить вращение камеры без наклонов
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	// Получить вектор движения вправо
	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	// Добавить движение влево/вправо
	AddMovementInput(RightVector, Value);
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::SpawnProjectile(TSubclassOf<ASBaseProjectile> ClassToSpawn)
{
	// check(ClassToSpawn);
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = TraceStart + GetControlRotation().Vector() * 5000.0f;

		// Type collision 
		FCollisionObjectQueryParams ObjQueryParams;
		ObjQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		// Ignore Player
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FHitResult Hit;
		FRotator ProjRotation;

		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjQueryParams, QueryParams);
		if (bBlockingHit)
		{
			// ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
			ProjRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, Hit.ImpactPoint);
		}
		else
		{
			// ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
			ProjRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEnd);
		}

		// FColor LineColor = bBlockingHit ? FColor::Red : FColor::Green;
		// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 2.0f, 0, 1.0f);
		// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 2.0f, 32, FColor::Red, false, 2.0f);

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void ASCharacter::PrimaryAttack()
{
	StartAttackEffect();

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if (ProjectileClass)
	{
		SpawnProjectile(ProjectileClass);
	}
}

void ASCharacter::SecondaryAttack()
{
	StartAttackEffect();

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::SecondaryAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ASCharacter::Dash()
{
	StartAttackEffect();

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ASCharacter::Dash_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::StartAttackEffect()
{
	PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}


// Debug
void ASCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::HealMaxSelf()
{
	AttributeComp->HealMaxHP();
}

// TODO: добавить хил макс хп или перезагрузкой или в компоненте здоровья HealMaxHp функцию добавить
// TODO: перенести читы в менеджер читов и сделать UI меню
