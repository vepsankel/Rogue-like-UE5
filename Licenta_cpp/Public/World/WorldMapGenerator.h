// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WorldCell.h"
#include "WorldGlobals.h"
#include "WorldRandom.h"

#include <algorithm>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

#include "WorldMapGenerator.generated.h"

struct MapGenerator {
public:
	MapGenerator(int size);

	bool IsExpandable();
	int GetExpandableCell(AWorldCell*& cell, int pos);
	void AddExpandableCell(AWorldCell* cell, int pos);

private:
	std::vector<AWorldCell*> ExpandableCells;
	std::vector<int> ExpandableCellsPos;
	int Size;
	int ExpandableCellsCount;
};

UCLASS()
class LICENTA_CPP_API AWorldMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldMapGenerator();

	int GenerateMap();
	//void WriteToFile(const char* filename);
	void SetSeed(unsigned int seed);

	AWorldCell* GetCell(int x, int y);

	bool IsExpandable();
	int GetExpandableCell(AWorldCell*& cell, int& pos);
	void AddExpandableCell(AWorldCell* cell, int pos);

	UPROPERTY(EditAnywhere)
		int Size;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	std::vector<AWorldCell*> ExpandableCells;
	std::vector<int> ExpandableCellsPos;

	int ExpandableCellsCount;

	unsigned int Seed = 0;

	int D2ToD1(int x, int y);
	void D1ToD2(int pos, int& x, int& y);

	bool IsCellInRange(int x, int y);
	bool IsCellExpandable(int x, int y);
	bool IsCellOfType(int x, int y, CellType type);

	void SetCell(int x, int y, AWorldCell* cell);
	void SetCellType(int x, int y, CellType type);

	const std::set<Direction> GetCellExpansionDir(int x, int y);

	TArray<AWorldCell*> Cells;
	MapGenerator* Generator;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};