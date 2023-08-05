// Fill out your copyright notice in the Description page of Project Settings.


#include "World/WorldMapCells.h"

WorldMapCells::WorldMapCells(unsigned int Size)
{
	this->Size = Size;
	Cells.Init(CELLTYPE_EMPTY, Size*Size);
}

WorldMapCells::~WorldMapCells()
{
}

unsigned int WorldMapCells::GetSize()
{
	return this->Size;
}

int WorldMapCells::D2ToD1(unsigned int x, unsigned int y) {
	return x * this->Size + y;
}

void WorldMapCells::D1ToD2(unsigned int pos, unsigned int& x, unsigned int& y) {
	x = pos / this->Size;
	y = pos % this->Size;
}

bool WorldMapCells::IsCellInRange(unsigned int x, unsigned int y) {
	return (x < this->Size&& x >= 0 && y < this->Size&& y >= 0);
}

bool WorldMapCells::IsCellOfType(unsigned int x, unsigned int y, CellType type) {
	if (!IsCellInRange(x, y))
		return false;

	return Cells[D2ToD1(x,y)] == type;
}

void WorldMapCells::SetCellType(unsigned int x, unsigned int y, CellType type) {
	UE_LOG(WorldGeneration, Log, TEXT("CellType for the Cell (%d, %d) is set to %d"), x, y, type);
	Cells[D2ToD1(x, y)] = type;
}