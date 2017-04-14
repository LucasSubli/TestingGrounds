// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TileController.generated.h"

UCLASS()
class TESTINGGROUNDS_API ATileController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileController();

	UFUNCTION(BlueprintCallable, Category = "Spawnables")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float MinScale = 1, float MaxScale = 1, float Radius = 500);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	bool CastSphere(FVector Location, float Radius);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);
	
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale);
	
	bool CanSpawnAtLocation(FVector Location, float Radius);
	
};
