// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <set>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/WorldCell.h"
#include "World/WorldRandom.h"
#include "World/WorldGlobals.h"
#include "World/WorldMapCells.h"
#include "WorldMazeGenerator.generated.h"

UCLASS()
class LICENTA_CPP_API AWorldMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldMazeGenerator();
	
	bool IsExpandable();
	int GenerateMaze(WorldMapCells * Cells);
	int GetExpandableCell(unsigned int &pos);
	void AddExpandableCell(unsigned int pos);

	UPROPERTY(EditAnywhere)
	unsigned int DistanceBetweenCorners = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	WorldMapCells * Cells;
	std::vector<int> ExpandableCellsPos;
	int ExpandableCellsCount;

	bool IsCellExpandable(int x, int y);
	const std::set<Direction> GetCellExpansionDir(unsigned int x, unsigned int y);
};
