// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant/PlantsManager.h"

#include "Plant/PlantsGlobals.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "World/WorldGlobals.h"

// Sets default values
APlantsManager::APlantsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

void APlantsManager::TickPlants()
{
	for (auto & PosPlantTuple : PlantsInstances)
	{
		const FIntVector3 Pos = PosPlantTuple.Key;
		FPlantInstance& Val = PosPlantTuple.Value;

		UE_LOG(PlantsLog, Log, TEXT("Plant with pos (%d,%d,%d)"), Pos.X, Pos.Y, Pos.Z);
		
		if (Val.Plant == nullptr)
		{
			UE_LOG(PlantsLog, Error, TEXT("Plant with pos (%d,%d,%d) exists but no such plant type is known"), Pos.X, Pos.Y, Pos.Z);
			continue;
		}

		if (Val.IsFinalStage)
		{
			UE_LOG(PlantsLog, Log, TEXT("Is final stage"));
			continue;
		}

		const unsigned int Stage = Val.Stage;
		const unsigned int RequiredTicksForNextStage = Val.Plant->PlantGrowthDuration[Stage];
		
		Val.ElapsedTicks++;

		UE_LOG(PlantsLog, Log, TEXT("Elapsed %u, Required %u"), Val.ElapsedTicks, RequiredTicksForNextStage);

		// Plant grows into next stage
		// Delete HISM instance for that stage and create for the next one
		if (Val.ElapsedTicks >= RequiredTicksForNextStage)
		{
			const unsigned int NextStage = Stage + 1;
			
			if (RemovePlantFromHism(Pos) == WORLD_FAILURE)
			{
				UE_LOG(PlantsLog, Log, TEXT("Removing plant (%d,%d,%d) from HISM failed when it grew into next stage"), Pos.X, Pos.Y, Pos.Z);
				continue;
			}

			int Instance;
			if (AddNewPlantToHism(Val.Plant, FVector(Pos), Instance, NextStage) == WORLD_FAILURE)
			{
				UE_LOG(PlantsLog, Log, TEXT("Adding plant (%d,%d,%d) to HISM failed when it grew into next stage"), Pos.X, Pos.Y, Pos.Z);
				continue;
			}

			Val.HISM_Id = Instance;
			Val.Stage = NextStage;
			Val.ElapsedTicks = 0;
			Val.IsFinalStage = Val.Plant->IsFinalStage(NextStage);
		}
	}
}


// Called when the game starts or when spawned
void APlantsManager::BeginPlay()
{
	Super::BeginPlay();

	// USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// RootComponent = RootComp;

	for (const auto Plant : Plants)
	{
		int Stage = 0;
		for (const auto PlantStage : Plant->PlantStages)
		{
			if (PlantStage == nullptr)
				break;
			
			FString PlantNameStageName = Plant->GetFullStageName(Stage);
			UHierarchicalInstancedStaticMeshComponent* HISM_Plant = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
			AddInstanceComponent(HISM_Plant);
			HISM_Plant->RegisterComponent();

			HISM_Plant->bMultiBodyOverlap = true;
			HISM_Plant->SetStaticMesh(PlantStage);
			HISM_Plant->SetCullDistances(0, 5000);
			HISM_Plants.Add(PlantNameStageName, HISM_Plant);

			UE_LOG(PlantsLog, Log, TEXT("Added plant stage %s"), *FString(PlantNameStageName));

			Stage++;
		}
	}
}

int APlantsManager::RemovePlantFromHism(FIntVector3 Pos)
{
	// Get the plant growing there
	FPlantInstance * PlantInstance = PlantsInstances.Find(Pos);

	if (PlantInstance == nullptr)
	{
		UE_LOG(PlantsLog, Error, TEXT("No plant exists for pos (%d,%d,%d)"), Pos.X, Pos.Y, Pos.Z);
		return WORLD_FAILURE;
	}

	const APlant * RemovedPlant = PlantInstance->Plant;
	unsigned int RemovedStage = PlantInstance->Stage;

	if (RemovedPlant == nullptr)
	{
		UE_LOG(PlantsLog, Error, TEXT("No plant exists for pos (%d,%d,%d)"), Pos.X, Pos.Y, Pos.Z);
		return WORLD_FAILURE;
	}

	// Get the HISM for this plant stage
	FString PlantNameStageName = RemovedPlant->GetFullStageName(RemovedStage);
	UHierarchicalInstancedStaticMeshComponent * HISM_Removed = HISM_Plants[PlantNameStageName];

	if (HISM_Removed == nullptr)
	{
		UE_LOG(PlantsLog, Error, TEXT("No hism component exists for %s"), *FString(PlantNameStageName));
		return WORLD_FAILURE;
	}

	// Get the plant id
	// FHitResult Result;
	// GetWorld()->LineTraceSingleByChannel(
	// 	Result,
	// 	FVector(Pos + FIntVector3(0,0,100)),
	// 	FVector(Pos + FIntVector3(0,0,-100)),
	// 	ECC_GameTraceChannel1
	// 	);
	//
	// int32 Item = Result.ElementIndex;

	FIntVector3 DeletedHismPos = FIntVector3(0);
	int DeletedHismIdx = -1;

	FIntVector3 BiggestIdxPos = FIntVector3(0);
	int BiggestHismIdx = -1;

	for (auto & PosToInstance : PlantsInstances)
	{
		FIntVector3 IterPos = PosToInstance.Key;
		FPlantInstance & Instance = PosToInstance.Value;

		if (Instance.Plant->GetFullStageName(Instance.Stage).Equals(PlantNameStageName))
		{
			if (Instance.HISM_Id > BiggestHismIdx)
			{
				BiggestHismIdx = Instance.HISM_Id;
				BiggestIdxPos = IterPos;
			}
		}
	}

	DeletedHismPos = Pos;
	DeletedHismIdx = PlantsInstances[Pos].HISM_Id;
	FTransform BiggestHismTransform;
	
	HISM_Removed->GetInstanceTransform(BiggestHismIdx,BiggestHismTransform);
	HISM_Removed->UpdateInstanceTransform(DeletedHismIdx, BiggestHismTransform, false);

	if (HISM_Removed->RemoveInstance(DeletedHismIdx) != true)
	{
		UE_LOG(PlantsLog, Error, TEXT("HISM corresponding to plant stage %s has no element of index %d"), *FString(PlantNameStageName), BiggestHismIdx);
		return WORLD_FAILURE;
	}

	PlantsInstances[BiggestIdxPos].HISM_Id = DeletedHismIdx;
	PlantsInstances[DeletedHismPos].HISM_Id = -1;

	return WORLD_SUCCESS;
}

int APlantsManager::AddNewPlantToHism(const APlant* Plant, FVector Pos, int & Index, unsigned Stage)
{
	FString FullName = Plant->GetFullStageName(Stage);
	FTransform Transform;
	FVector Translation(Pos.X, Pos.Y, 0);
	Transform.AddToTranslation(Translation);

	UE_LOG(PlantsLog, Log, TEXT("Try to add plant stage with name %s"), *FString(FullName));

	UHierarchicalInstancedStaticMeshComponent * HISM_Plant = HISM_Plants[FullName];

	if (HISM_Plant == nullptr)
	{
		UE_LOG(PlantsLog, Error, TEXT("No HISM for plantstge %s exist!"), *FString(FullName));
		return WORLD_FAILURE;
	}

	Index = HISM_Plant->AddInstance(Transform);

	return WORLD_SUCCESS;
}

const APlant * APlantsManager::RemovePlant(FIntVector3 Pos)
{
	if (RemovePlantFromHism(Pos) != WORLD_SUCCESS)
	{
		UE_LOG(PlantsLog, Error, TEXT("Could not remove plant (%d,%d,%d) from HISM"), Pos.X, Pos.Y, Pos.Z);
		return nullptr;
	}

	FPlantInstance * PlantInstance = PlantsInstances.Find(Pos);

	if (PlantInstance == nullptr)
	{
		return nullptr;
	}
	
	const APlant * Plant = PlantInstance->Plant;
	PlantsInstances.Remove(Pos);
	
	return Plant;
}

bool APlantsManager::IsPlantFullyGrown(FIntVector3 Pos)
{
	FPlantInstance * PlantInstance = PlantsInstances.Find(Pos);

	if (PlantInstance == nullptr)
		return false;

	return PlantInstance->IsFinalStage;
}

int APlantsManager::AddNewPlant(const APlant* Plant, FVector Pos, unsigned int Stage, unsigned int ElapsedTicks)
{
	int Index = 0;
	if (AddNewPlantToHism(Plant, Pos, Index, Stage) != WORLD_SUCCESS)
	{
		UE_LOG(PlantsLog, Error, TEXT("Could not add plant (%d,%d,%d), Stage %d to HISM"), Pos.X, Pos.Y, Pos.Z, Stage);
		RemovePlantFromHism(FIntVector3(Pos.X, Pos.Y, Pos.Z));
		return WORLD_FAILURE;
	}
	
	FPlantInstance NewPlant = {Index, Plant, Stage, ElapsedTicks};
	
	while (ElapsedTicks >= Plant->PlantGrowthDuration[Stage] && !Plant->IsFinalStage(Stage))
	{
		ElapsedTicks -= Plant->PlantGrowthDuration[Stage];
		Stage++;
	}
	
	PlantsInstances.Add(TTuple<FVector, FPlantInstance>(FIntVector3(Pos.X, Pos.Y, Pos.Z), NewPlant));
	
	return WORLD_SUCCESS;
}


// Called every frame
void APlantsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

