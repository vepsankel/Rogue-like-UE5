// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Plant.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantsManager.generated.h"

struct FPlantInstance
{
	int32 HISM_Id;
	const APlant* Plant;
	unsigned int Stage;
	unsigned int ElapsedTicks;
	bool IsFinalStage;
};

UCLASS()
class LICENTA_CPP_API APlantsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantsManager();

	UPROPERTY(EditAnywhere)
	TArray<APlant *> Plants;

	void TickPlants();

	int AddNewPlant(const APlant* Plant, FVector Pos, unsigned int Stage = 0, unsigned int ElapsedTicks = 0);
	int RemovePlant(FIntVector3 Pos);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TMap<FString, UHierarchicalInstancedStaticMeshComponent *> HISM_Plants;
	TMap<FIntVector3, FPlantInstance> PlantsInstances;

	int RemovePlantFromHism(FIntVector3);
	int AddNewPlantToHism(const APlant* Plant, FVector Pos, unsigned int Stage = 0);
};
