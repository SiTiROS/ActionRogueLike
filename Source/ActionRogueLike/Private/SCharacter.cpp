#include "SCharacter.h"
#include "SMagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Настройка, чтобы вращать пружинной палкой с камерой по оси Y
	SpringArmComp->bUsePawnControlRotation = true;

	// Отвязываем вращение персонажа мышкой
	bUseControllerRotationYaw = false;

	// Ориентируем поворот персонажа в сторону движения 
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

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

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
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

void ASCharacter::PrimaryAttack()
{
	FVector BoneLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(), BoneLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}
