#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class ASAICharacter;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameModeBase();

	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

protected:
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|AI")
	TSubclassOf<ASAICharacter> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup|Credits")
	int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Pickups")
	UEnvQuery* PickupSpawnQuery;

	/* All pickup classes used to spawn with EQS at match start */
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Pickups")
	TArray<TSubclassOf<AActor>> PickupClasses;

	/* Distance required between pickup spawn locations */
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Pickups")
	float RequiredPickupDistance;

	/* Amount of pickup to spawn during match start */
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Pickups")
	int32 DesiredPickupCount;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
	void OnPickupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:
	//debug
	UFUNCTION(Exec) // доступна из консоли
	void KillAll();
};
