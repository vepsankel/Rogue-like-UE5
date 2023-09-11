// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant/Plant.h"

#include "PlantsGlobals.h"

// Sets default values
APlant::APlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APlant::BeginPlay()
{
	Super::BeginPlay();
}

bool APlant::IsFinalStage(const int Stage) const
{
	return (Stage + 1 >= PlantStages.Num()
		|| (PlantStages[Stage+1] == nullptr)
		|| (PlantGrowthDuration[Stage] == 0));
}

FString APlant::GetFullStageName(const int Stage) const
{
	return PlantName + PlantStages[Stage]->GetName();
}

// Called every frame
void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

