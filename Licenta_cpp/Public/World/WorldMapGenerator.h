// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <algorithm>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

#include "WorldRoomGenerator.h"
#include "Plant/PlantsManager.h"
#include "World/WorldCell.h"
#include "World/WorldGlobals.h"
#include "World/WorldRandom.h"
#include "World/WorldMapCells.h"
#include "World/WorldMazeGenerator.h"
#include "WorldMapGenerator.generated.h"

UCLASS()
class LICENTA_CPP_API AWorldMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldMapGenerator();
	
	void SetSeed(unsigned int seed);
	
	int GenerateMap();
	int BuildMap();
	int TickPlants();

	UPROPERTY(EditAnywhere)
		int Size;

	UPROPERTY(EditAnywhere)
		unsigned int Seed;

	UPROPERTY(EditAnywhere)
		float SoilPercentage;

	UPROPERTY(EditAnywhere)
		AActor* WallStaticMeshActor;

	UPROPERTY(EditAnywhere)
		AActor* RoomActor;

	UPROPERTY(EditAnywhere)
		AWorldMazeGenerator* MazeGenerator;

	UPROPERTY(EditAnywhere)
		AWorldRoomGenerator* RoomGenerator;

	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent* HISM_Maze;

	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent* HISM_Soil;

	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
		APlantsManager *PlantsManager;

	int GetWorldMapCells(WorldMapCells ** Cells);
	int GetPlantsManager(APlantsManager** PlantsManager);
	int GetPlayerSpawnRoomPos(FVector & Pos);

	int AddNewPlant(const APlant* , FVector);
	int ReapPlant(FVector Pos, const APlant ** Plant);
	bool IsPlantFullyGrown(FIntVector Pos);

	bool IsCellOfType(unsigned int, unsigned int, CellType) const; 
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	WorldMapCells Cells = WorldMapCells(Size);
	unsigned int HISM_Soil_Size = 0;
	TMap<int32, FTransform> IdToSoilTransform;
	
	unsigned int HIST_Maze_Size = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};