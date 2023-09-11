// Fill out your copyright notice in the Description page of Project Settings.
#include "World/WorldMazeGenerator.h"

// Sets default values
AWorldMazeGenerator::AWorldMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWorldMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWorldMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AWorldMazeGenerator::GenerateMaze(WorldMapCells * MapCells)
{
	this->Cells = MapCells;
	unsigned int size = Cells->GetSize();
	
	UE_LOG(WorldMazeGeneration, Log, TEXT("Map size is %u"), size);
	// check the size is set
	if (size <= 0) {
		UE_LOG(WorldMazeGeneration, Warning, TEXT("Map generation ended due to unexpected size"));
		return -1;
	}

	// ExpandableCells.resize(size * size);
	ExpandableCellsPos.resize(size * size);
	ExpandableCellsCount = 0;

	// first cell for maze generation
	// first cells coords
	int firstWallX = size / 2;
	int firstWallY = size / 2;

	UE_LOG(Player, Error, TEXT("FirstWallX %d, FirstWallY %d"), firstWallX, firstWallY);
	
	AddExpandableCell(Cells->D2ToD1(firstWallX, firstWallY));
	Cells->SetCellType(firstWallX, firstWallY, CELLTYPE_OBSTACLE);

	//tmp
	for (unsigned int BorderX = 0; BorderX < size; BorderX++)
	{
		Cells->SetCellType(BorderX, 0, CELLTYPE_OBSTACLE);

		if (abs((int)(firstWallX - BorderX)) % DistanceBetweenCorners == 0 && (abs((int)(firstWallY)) % DistanceBetweenCorners == 0))
		{
			
			unsigned int i;
			Random(i);
			if (i % 3 == 0)
			{
				AddExpandableCell(Cells->D2ToD1(BorderX, 0));
			} 
		}
	}

	for (unsigned int BorderX = 0; BorderX < size; BorderX++)
	{
		Cells->SetCellType(BorderX, size-1, CELLTYPE_OBSTACLE);
	
		if (abs((int)(firstWallX - BorderX)) % DistanceBetweenCorners == 0 && (abs((int)((size-1) - firstWallY)) % DistanceBetweenCorners == 0))
		{
			unsigned int i;
			Random(i);
			if (i % 3 == 0)
			{
				AddExpandableCell(Cells->D2ToD1(BorderX, size - 1));
			} 
		}
	}

	for (unsigned int BorderY = 0; BorderY < size; BorderY++)
	{
		Cells->SetCellType(0, BorderY, CELLTYPE_OBSTACLE);
	
		if (abs((int)(firstWallX)) % DistanceBetweenCorners == 0 && (abs((int)(BorderY - firstWallY)) % DistanceBetweenCorners == 0))
		{
			unsigned int i;
			Random(i);
			if (i % 3 == 0)
			{
				AddExpandableCell(Cells->D2ToD1(0, BorderY));
			} 
		}
	}

	for (unsigned int BorderY = 0; BorderY < size; BorderY++)
	{
		Cells->SetCellType(size - 1, BorderY, CELLTYPE_OBSTACLE);
	
		if (abs((int)((size - 1) - firstWallX)) % DistanceBetweenCorners == 0 && (abs((int)(BorderY - firstWallY)) % DistanceBetweenCorners == 0))
		{
			unsigned int i;
			Random(i);
			if (i % 3 == 0)
			{
				AddExpandableCell(Cells->D2ToD1(size - 1, BorderY));
			} 
		}
	}
	//tmp

	unsigned int cellIdx = 0;
	unsigned int cellPos = 0;
	unsigned int cellPosX = 0;
	unsigned int cellPosY = 0;
	while (IsExpandable()) {
		// UE_LOG(WorldMazeGeneration, Log, TEXT("Map can be expanded"));
		
		cellIdx = GetExpandableCell(cellPos);
		Cells->D1ToD2(cellPos, cellPosX, cellPosY);

		// UE_LOG(WorldMazeGeneration, Log, TEXT("The expandable wall was selected. Coords = %d (x=%d, y=%d)"), cellPos, cellPosX, cellPosY);

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

		// UE_LOG(WorldMazeGeneration, Log, TEXT("The selected expandion direction = %d"), randomDir);

		// Expand walls acording to selected random direction
		for (unsigned int k = 1; k <= DistanceBetweenCorners; k++) {
			Cells->SetCellType(cellPosX + k * randomDirX, cellPosY + k * randomDirY, CELLTYPE_OBSTACLE);
		}

		// Add cells that can be expandable
		if (IsCellExpandable(cellPosX, cellPosY))
		{
			AddExpandableCell(Cells->D2ToD1(cellPosX, cellPosY));
			// UE_LOG(WorldMazeGeneration, Log, TEXT("The selected cell can be expanded further."));
		}
		if (IsCellExpandable(cellPosX + DistanceBetweenCorners * randomDirX, cellPosY + DistanceBetweenCorners * randomDirY))
		{
			AddExpandableCell(Cells->D2ToD1(cellPosX + DistanceBetweenCorners * randomDirX, cellPosY + DistanceBetweenCorners * randomDirY));
			// UE_LOG(WorldMazeGeneration, Log, TEXT("The expanded cell can be expanded further."));
		}
	}
	
	return 0;
}

bool AWorldMazeGenerator::IsExpandable() {
	return ExpandableCellsCount != 0;
}

int AWorldMazeGenerator::GetExpandableCell(unsigned int &pos) {
	// randomize
	unsigned int randomPos;
	Random(randomPos);
	randomPos %= ExpandableCellsCount;

	// cell = this->ExpandableCells.at(randomPos);
	// this->ExpandableCells.erase(ExpandableCells.begin() + randomPos);
	pos = this->ExpandableCellsPos.at(randomPos);
	this->ExpandableCellsPos.erase(ExpandableCellsPos.begin() + randomPos);
	ExpandableCellsCount--;
	return randomPos;
}

void AWorldMazeGenerator::AddExpandableCell(unsigned int pos) {
	// this->ExpandableCells[ExpandableCellsCount] = cell;
	this->ExpandableCellsPos[ExpandableCellsCount] = pos;
	ExpandableCellsCount++;

	// UE_LOG(WorldMazeGeneration, Log, TEXT("Added expendable cell pos=%d. There are %d expandable cells."), pos, ExpandableCellsCount);
}

bool AWorldMazeGenerator::IsCellExpandable(int x, int y) {
	return GetCellExpansionDir(x, y).size() != 0;
}

const std::set<Direction> AWorldMazeGenerator::GetCellExpansionDir(unsigned int x, unsigned int y) {
	std::set<Direction> directions;
	std::vector<Direction> iterDirections = { DIRECTION_UP, DIRECTION_DOWN, DIRECTION_RIGHT, DIRECTION_LEFT };

	unsigned int iterDirectionsSize = iterDirections.size();

	for (unsigned int dir = 0; dir < iterDirectionsSize; dir++) {
		Direction expansionDir = iterDirections[dir];

		int expansionX = x + DistanceBetweenCorners * DirToX(expansionDir);
		int expansionY = y + DistanceBetweenCorners * DirToY(expansionDir);

		if (Cells->IsCellOfType(expansionX, expansionY, CELLTYPE_EMPTY)) {
			directions.insert(expansionDir);
		}
	}

	return directions;
}