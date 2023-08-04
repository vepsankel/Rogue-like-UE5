// Fill out your copyright notice in the Description page of Project Settings.


#include "World/WorldMapGenerator.h"

// Sets default values
AWorldMapGenerator::AWorldMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Random::SetSeed(this->Seed);

	// deallocate previous generator
	if (this->Generator != nullptr) {
		free(this->Generator);
	}
	Cells.Empty();

	// check the size is set
	if (this->Size <= 0) {
		return;
	}

	// fill the array with default cells
	const int size = this->Size;
	Cells.SetNumUninitialized(size * size, false);
	FVector Location(0, 0, 0);
	for (int i = 0; i < size * size; i++) {
		AWorldCell *cell = (AWorldCell*) GetWorld()->SpawnActor(AWorldCell::StaticClass());
	}
	ExpandableCells.resize(size * size);
	ExpandableCellsPos.resize(size * size);
	ExpandableCellsCount = 0;

	// first cell for maze generation
	// first cells coords
	int firstWallX = size / 2;
	int firstWallY = size / 2;
	this->Generator = new MapGenerator(size);
	this->Generator->AddExpandableCell(GetCell(firstWallX, firstWallY), D2ToD1(firstWallX, firstWallY));
	SetCellType(firstWallX, firstWallY, CELLTYPE_OBSTACLE);

	int cellIdx = 0;
	int cellPos = 0;
	int cellPosX = 0;
	int cellPosY = 0;
	while (this->Generator->IsExpandable()) {
		AWorldCell* expandableCell;
		cellIdx = this->Generator->GetExpandableCell(expandableCell, cellPos);
		D1ToD2(cellPos, cellPosX, cellPosY);

		// Expand:
		// Select a random direction to expand this wall
		std::set<Direction> expandableDirs = GetCellExpansionDir(cellPosX, cellPosY);
		int expandableDirsSize = expandableDirs.size();

		if (expandableDirsSize == 0)
			continue;

		unsigned int cellExpansionDirIdx = 0;

		Random(cellExpansionDirIdx);
		cellExpansionDirIdx %= expandableDirsSize;

		auto randomDirIt = std::begin(expandableDirs);
		std::advance(randomDirIt, cellExpansionDirIdx);

		Direction randomDir = *randomDirIt;
		int randomDirX = DirToX(randomDir);
		int randomDirY = DirToY(randomDir);

		// Expand walls acording to selected random direction
		for (int k = 1; k <= 2; k++) {
			SetCellType(cellPosX + k * randomDirX, cellPosY + k * randomDirY, CELLTYPE_OBSTACLE);
		}

		// Add cells that can be expandable
		if (IsCellExpandable(cellPosX, cellPosY))
			this->Generator->AddExpandableCell(GetCell(cellPosX, cellPosY), D2ToD1(cellPosX, cellPosY));
		if (IsCellExpandable(cellPosX + 2 * randomDirX, cellPosY + 2 * randomDirY))
			this->Generator->AddExpandableCell(GetCell(cellPosX + 2 * randomDirX, cellPosY + 2 * randomDirY),
				D2ToD1(cellPosX + 2 * randomDirX, cellPosY + 2 * randomDirY));
	}

	Random::RemoveSeed();
}

// Called when the game starts or when spawned
void AWorldMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AWorldMapGenerator::D2ToD1(int x, int y) {
	return x * this->Size + y;
}

void AWorldMapGenerator::D1ToD2(int pos, int& x, int& y) {
	x = pos / this->Size;
	y = pos % this->Size;
}

void AWorldMapGenerator::SetSeed(unsigned int seed) {
	this->Seed = seed;
}

bool AWorldMapGenerator::IsCellInRange(int x, int y) {
	return (x < this->Size&& x >= 0 && y < this->Size&& y >= 0);
}

bool AWorldMapGenerator::IsCellExpandable(int x, int y) {
	return GetCellExpansionDir(x, y).size() != 0;
}

bool AWorldMapGenerator::IsCellOfType(int x, int y, CellType type) {
	if (!IsCellInRange(x, y))
		return false;

	return GetCell(x, y)->GetType() == type;
}

void AWorldMapGenerator::SetCell(int x, int y, AWorldCell* cell) {
	Cells[D2ToD1(x, y)] = cell;
}

void AWorldMapGenerator::SetCellType(int x, int y, CellType type) {
	Cells[D2ToD1(x, y)]->SetType(type);
}

AWorldCell* AWorldMapGenerator::GetCell(int x, int y) {
	return Cells[D2ToD1(x, y)];
}

const std::set<Direction> AWorldMapGenerator::GetCellExpansionDir(int x, int y) {
	std::set<Direction> directions;
	std::vector<Direction> iterDirections = { DIRECTION_UP, DIRECTION_DOWN, DIRECTION_RIGHT, DIRECTION_LEFT };

	unsigned int iterDirectionsSize = iterDirections.size();

	for (unsigned int dir = 0; dir < iterDirectionsSize; dir++) {
		Direction expansionDir = iterDirections[dir];

		int expansionX = x + 2 * DirToX(expansionDir);
		int expansionY = y + 2 * DirToY(expansionDir);

		if (IsCellOfType(expansionX, expansionY, CELLTYPE_EMPTY)) {
			directions.insert(expansionDir);
		}
	}

	return directions;
}

bool AWorldMapGenerator::IsExpandable() {
	return ExpandableCellsCount != 0;
}

int AWorldMapGenerator::GetExpandableCell(AWorldCell*& cell, int& pos) {
	// randomize
	unsigned int randomPos;
	Random(randomPos);
	randomPos %= ExpandableCellsCount;

	cell = this->ExpandableCells.at(randomPos);
	this->ExpandableCells.erase(ExpandableCells.begin() + randomPos);
	pos = this->ExpandableCellsPos.at(randomPos);
	this->ExpandableCellsPos.erase(ExpandableCellsPos.begin() + randomPos);
	ExpandableCellsCount--;
	return randomPos;
}

void AWorldMapGenerator::AddExpandableCell(AWorldCell* cell, int pos) {
	this->ExpandableCells[ExpandableCellsCount] = cell;
	this->ExpandableCellsPos[ExpandableCellsCount] = pos;
	ExpandableCellsCount++;
}

MapGenerator::MapGenerator(int size) {
	this->Size = size;
	this->ExpandableCells.resize(size * size);
	this->ExpandableCellsPos.resize(size * size);
	this->ExpandableCellsCount = 0;
}

bool MapGenerator::IsExpandable() {
	return ExpandableCellsCount != 0;
}

int MapGenerator::GetExpandableCell(AWorldCell*& cell, int pos) {
	// randomize
	unsigned int randomPos;
	Random(randomPos);
	randomPos %= ExpandableCellsCount;

	cell = this->ExpandableCells.at(randomPos);
	this->ExpandableCells.erase(ExpandableCells.begin() + randomPos);
	pos = this->ExpandableCellsPos.at(randomPos);
	this->ExpandableCellsPos.erase(ExpandableCellsPos.begin() + randomPos);
	ExpandableCellsCount--;
	return randomPos;
}

void MapGenerator::AddExpandableCell(AWorldCell* cell, int pos) {
	this->ExpandableCells[ExpandableCellsCount] = cell;
	this->ExpandableCellsPos[ExpandableCellsCount] = pos;
	ExpandableCellsCount++;
}