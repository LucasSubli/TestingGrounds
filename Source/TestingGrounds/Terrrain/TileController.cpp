// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGrounds.h"
#include "TileController.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"


// Sets default values
ATileController::ATileController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavigationBoundsOffset = FVector(2000, 0, 0);

	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);

}

// Called when the game starts or when spawned
void ATileController::BeginPlay() {
	Super::BeginPlay();

	/*CastSphere(GetActorLocation(), 300);
	CastSphere(GetActorLocation() + FVector(0, 0, 1000), 300);*/

}

void ATileController::EndPlay(const EEndPlayReason::Type EndPlayReason) {

	UE_LOG(LogTemp, Warning, TEXT("[%s] Endplay"), *GetName());
	Pool->Return(NavMeshBoundsVolume);
	UE_LOG(LogTemp, Warning, TEXT("[%s] Returned: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ATileController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ATileController::SetPool(UActorPool * InPool) {
	Pool = InPool;
	PositionNavMeshBoundsVolume();
}

void ATileController::PositionNavMeshBoundsVolume() {

	NavMeshBoundsVolume = Pool->Checkout();

	if (NavMeshBoundsVolume == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool."), *GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

template<class T>
void ATileController::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float MinScale, float MaxScale, float Radius) {

	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++) {

		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);

		bool found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		if (found) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

TArray<FSpawnPosition> ATileController::RandomSpawnPositions(int MinSpawn, int MaxSpawn, float MinScale, float MaxScale, float Radius, TArray<FSpawnPosition> &SpawnPositions) {
	
	
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++) {

		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);

		bool found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		if (found) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			SpawnPositions.Add(SpawnPosition);
		}
	}

	return SpawnPositions;
}

void ATileController::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float MinScale, float MaxScale, float Radius) {

	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, MinScale, MaxScale, Radius);
}

void ATileController::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius) {

	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, 1, 1, Radius);
}

bool ATileController::FindEmptyLocation(FVector& OutLocation, float Radius) {
	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPTS = 100;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
			if (CanSpawnAtLocation(CandidatePoint, Radius)) {
				OutLocation = CandidatePoint;
				return true;
			}
	}
	return false;
}


bool ATileController::CanSpawnAtLocation(FVector Location, float Radius) {
	{
		FHitResult HitResult;
		FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
		bool HasHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			GlobalLocation,
			GlobalLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(Radius)
		);
		//FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
		//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
		return !HasHit;
	}
}

void ATileController::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition) {
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned) {
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}
}

void ATileController::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition) {
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	if (Spawned) {
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
	}
}


bool ATileController::CastSphere(FVector Location, float Radius) {
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Location,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
		);
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), Location, 0, Radius, FQuat::Identity, ResultColor, true, 100);
	return HasHit;
}

