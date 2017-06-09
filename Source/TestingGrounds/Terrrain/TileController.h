// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TileController.generated.h"

/**
 * Struct for position for random props and AI
 */
USTRUCT()
struct FSpawnPosition {
	GENERATED_USTRUCT_BODY()

	FVector Location;

	float Rotation; 

	float Scale;

};


class UActorPool;

UCLASS()
class TESTINGGROUNDS_API ATileController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileController();

	UFUNCTION(BlueprintCallable, Category = "Spawnables")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float MinScale = 1, float MaxScale = 1, float Radius = 500);

	UFUNCTION(BlueprintCallable, Category = "Spawnables")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FVector MinExtent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Setter for pool
	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);

private:

	bool CastSphere(FVector Location, float Radius);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float MinScale = 1, float MaxScale = 1, float Radius = 500);
	
	void PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition);

	void PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition);
	
	bool CanSpawnAtLocation(FVector Location, float Radius);

	void PositionNavMeshBoundsVolume();

	TArray<FSpawnPosition> RandomSpawnPositions(int MinSpawn, int MaxSpawn, float MinScale, float MaxScale, float Radius, TArray<FSpawnPosition> &SpawnPositions);

	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;
	
};
