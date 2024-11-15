#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SBaseProjectile.h"

ASCharacter::ASCharacter()
	: AttackAnimDelay(0.2f)
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

void ASCharacter::SpawnProjectile(TSubclassOf<ASBaseProjectile> ClassToSpawn)
{
	// check(ClassToSpawn);
	if (ensure(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

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
	PlayAnimMontage(AttackAnim);

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
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::SecondaryAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);

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
