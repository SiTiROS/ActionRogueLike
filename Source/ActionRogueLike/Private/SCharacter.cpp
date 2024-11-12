#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
