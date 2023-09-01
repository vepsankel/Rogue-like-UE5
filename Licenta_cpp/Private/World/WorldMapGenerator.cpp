// Fill out your copyright notice in the Description page of Project Settings.
#include "World/WorldMapGenerator.h"

#include "IPropertyTable.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Plant/PlantsGlobals.h"

// Sets default values
AWorldMapGenerator::AWorldMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Cells = WorldMapCells(Size);
}

// Called when the game starts or when spawned
void AWorldMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	if (this->GenerateMap() != WORLD_SUCCESS)
	{
		UE_LOG(WorldGeneration, Error, TEXT("Could not generate maps"));
	}
	
	this->BuildMap();
}

int AWorldMapGenerator::GenerateMap() {
	Random::SetSeed(this->Seed);
	UE_LOG(WorldGeneration, Log, TEXT("Map Generation started with seed %u"), this->Seed);

	Cells.Initialise(Size);

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

	for (int x = 0 ; x < Size ; x++)
	{
		for (int y = 0 ; y < Size ; y++)
		{
			unsigned int random;
			
			if (Cells.IsCellOfType(x,y,CELLTYPE_EMPTY)) {
				Random(random);

				if ((float) random / (float)UINT_MAX < SoilPercentage)
				{
					Cells.SetCellType(x,y,CELLTYPE_SOIL_NO_PLANT);
				}
			}
		}
	}

	Random::RemoveSeed();
	return 0;
}

int AWorldMapGenerator::BuildMap()
{

	if (HISM_Maze != nullptr && Mesh != nullptr)
	{
		HISM_Maze->SetStaticMesh(Mesh);
	} else
	{
		return -1;
	}

	// cells
	for (int i = 0 ; i < Size * Size ; i++)
	{
		unsigned int cellCoordX, cellCoordY;
		Cells.D1ToD2(i, cellCoordX, cellCoordY);

		if (Cells.IsCellOfType(cellCoordX, cellCoordY, CELLTYPE_OBSTACLE))
		{
			FVector Translation(cellCoordX * 100, cellCoordY * 100, 0);
			FTransform Transform;
			Transform.AddToTranslation(Translation);
			int32 Id = HISM_Maze->AddInstance(Transform);
			IdToSoilTransform.Add(Id, Transform);
			HIST_Maze_Size++;
		}

		if (Cells.IsCellOfType(cellCoordX, cellCoordY, CELLTYPE_SOIL_NO_PLANT))
		{
			FVector Translation(cellCoordX * 100, cellCoordY * 100, -40);
			FTransform Transform;
			Transform.AddToTranslation(Translation);
			HISM_Soil->AddInstance(Transform);
			HISM_Soil_Size++;
		}
	}

	//rooms
	const TArray<unsigned int> * RoomsPos = Cells.GetRoomPos();
	const TArray<unsigned int> * RoomsSize = Cells.GetRoomSize();
	const TArray<ERoomType> * RoomType = Cells.GetRoomType();

	if (RoomActor != NULL)
		for (int i = 0 ; i < RoomsPos->Num() ; i++)
		{
			FActorSpawnParameters Params;
			
			unsigned int RoomPos = (*RoomsPos)[i];
			unsigned int RoomSize = (*RoomsSize)[i];

			unsigned int RoomPosX, RoomPosY;
			Cells.D1ToD2(RoomPos, RoomPosX, RoomPosY);
		
			FVector Translation(RoomPosX * 100, RoomPosY * 100, 300);
			FTransform Transform;
			Transform.AddToTranslation(Translation);
			Transform.SetScale3D(UE::Math::TVector<double>(RoomSize, RoomSize, RoomSize));
			GetWorld()->SpawnActor(
					RoomActor->GetClass(),
					&Transform,
					Params
				);

			UE_LOG(WorldGeneration, Log, TEXT("Spawning room..."));
		}

	return 0;
}

int AWorldMapGenerator::TickPlants()
{
	UE_LOG(WorldGeneration, Log, TEXT("Ticking plants"));
	
	if (PlantsManager == nullptr)
	{
		UE_LOG(WorldGeneration, Error, TEXT("Could not tick plants: no PlantsManager assigned"));
		return WORLD_FAILURE;
	}

	PlantsManager->TickPlants();
	return WORLD_SUCCESS;
}

// Called every frame
void AWorldMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldMapGenerator::SetSeed(unsigned int seed) {
	this->Seed = seed;
}

int AWorldMapGenerator::GetWorldMapCells(WorldMapCells** WorldCells)
{
	*WorldCells = &this->Cells;
	return WORLD_SUCCESS;
}

int AWorldMapGenerator::GetPlantsManager(APlantsManager** PPlantsManager)
{
	*PPlantsManager = this->PlantsManager;
	return WORLD_SUCCESS;
}

bool AWorldMapGenerator::IsCellOfType(unsigned int x, unsigned int y, CellType type) const {
	if (!Cells.IsCellInRange(x,y))
		return false;
	
	return Cells.IsCellOfType(x,y,type);
}

int AWorldMapGenerator::AddNewPlant(const APlant* Plant, FVector Pos)
{
	if (PlantsManager == nullptr)
	{
		UE_LOG(WorldGeneration, Error, TEXT("No PlantManager"));
		return WORLD_FAILURE;
	}

	int LocalX = ((int)Pos.X + 50) / 100;
	int LocalY = ((int)Pos.Y + 50) / 100;
	int GLobalX = 100 * LocalX;
	int GlobalY = 100 * LocalY;
	
	if (PlantsManager->AddNewPlant(Plant, FVector(GLobalX, GlobalY, 0)) == WORLD_SUCCESS)
	{
		Cells.SetCellType(LocalX, LocalY, CELLTYPE_SOIL_PLANT);
		return WORLD_SUCCESS;
	} else {
		UE_LOG(WorldGeneration, Error, TEXT("Could not add new plant"));
		return WORLD_FAILURE;
	}
}
