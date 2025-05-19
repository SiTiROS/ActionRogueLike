#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SActionComponent.h"
#include "Components/SAttributeComponent.h"
#include "Components/SInteractionComponent.h"
#include "Tests/Components/SInventoryComponent.h"

static TAutoConsoleVariable<bool> CVarDrawDebugRotationVisualization(TEXT("su.CharacterRotationDebugDraw"),
                                                                     false, TEXT("Enable Debug for Projectile. "), ECVF_Cheat);

ASCharacter::ASCharacter()
	: TimeToHitParamName("TimeToHit")
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

	InventoryComp = CreateDefaultSubobject<USInventoryComponent>(TEXT("InventoryComp"));
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged); // привязка функции к делегату
}

// Переопределяем базовую функцию, чтобы скорректировать для нашего вида от 3его лица для GetActorEyesViewPoint версии для pawn
FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bool bDrawDebug = CVarDrawDebugRotationVisualization.GetValueOnGameThread())
	{
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

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::SecondaryAttack()
{
	ActionComp->StartActionByName(this, "BlackHole");
}

void ASCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		AttributeComp->ApplyRage(InstigatorActor, FMath::Abs(Delta));
	}

	// Death
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		// Set lifespan
		SetLifeSpan(3.0f);
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
