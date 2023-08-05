// Fill out your copyright notice in the Description page of Project Settings.
#include "World/WorldMapGenerator.h"

#include "IPropertyTable.h"

// Sets default values
AWorldMapGenerator::AWorldMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWorldMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	Cells = WorldMapCells(Size);
	
	if (this->GenerateMap() != WORLD_SUCCESS)
	{
		return;
	}
	
	this->BuildMap();
}

int AWorldMapGenerator::GenerateMap() {
	Random::SetSeed(this->Seed);
	UE_LOG(WorldGeneration, Log, TEXT("Map Generation started with seed %u"), this->Seed);

	if (MazeGenerator != NULL)
		MazeGenerator->GenerateMaze(&Cells);
	else
	{
		UE_LOG(WorldGeneration, Error, TEXT("Maze could not be generated: no generator!"));
		return -1;
	}

	if (RoomGenerator != NULL)
	{
		RoomGenerator->GenerateRooms(&Cells);
	} else
	{
		UE_LOG(WorldGeneration, Error, TEXT("Rooms could not be generated: no generator!"));
		return -1;
	}

	Random::RemoveSeed();
	return 0;
}

int AWorldMapGenerator::BuildMap()
{
	FActorSpawnParameters Params;
	
	for (int i = 0 ; i < Size*Size ; i++)
	{
		unsigned int cellCoordX, cellCoordY;
		Cells.D1ToD2(i, cellCoordX, cellCoordY);
		UE_LOG(LogTemp, Warning, TEXT("Try to spawn..."));

		FVector Translation(cellCoordX * 100, cellCoordY * 100, 0);
		FTransform Transform;
		Transform.AddToTranslation(Translation);

		if (Cells.IsCellOfType(cellCoordX, cellCoordY, CELLTYPE_OBSTACLE))
		{
			(AWorldCell*)GetWorld()->SpawnActor(
				WallStaticMeshActor->GetClass(),
				&Transform,
				Params
			);
			// UE_LOG(LogTemp, Warning, TEXT("Obstacle!"));
		} else if (Cells.IsCellOfType(cellCoordX, cellCoordY, CELLTYPE_DEBUG))
		{
			Transform.AddToTranslation(FVector(0,0,200));
			(AWorldCell*)GetWorld()->SpawnActor(
				WallStaticMeshActor->GetClass(),
				&Transform,
				Params
			);
		}
	}

	return 0;
}

// Called every frame
void AWorldMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldMapGenerator::SetSeed(unsigned int seed) {
	this->Seed = seed;
}