// Fill out your copyright notice in the Description page of Project Settings.


#include "World/WorldMapCells.h"

WorldMapCells::WorldMapCells(unsigned int Size)
{
	Initialise(Size);
}

WorldMapCells::~WorldMapCells()
{
}

void WorldMapCells::Initialise(unsigned int CellsSize)
{
	this->Size = CellsSize;
	Cells.Init(CELLTYPE_EMPTY, CellsSize*CellsSize);
}

unsigned int WorldMapCells::GetSize()
{
	return this->Size;
}

int WorldMapCells::D2ToD1(unsigned int x, unsigned int y) const {
	return x * this->Size + y;
}

void WorldMapCells::D1ToD2(unsigned int pos, unsigned int& x, unsigned int& y) const {	
	x = pos / this->Size;
	y = pos % this->Size;
}

bool WorldMapCells::IsCellInRange(unsigned int x, unsigned int y) const {
	return (x < this->Size && y < this->Size);
}

bool WorldMapCells::IsCellOfType(unsigned int x, unsigned int y, CellType type) const {
	if (!IsCellInRange(x,y))
		return false;
	
	return Cells[D2ToD1(x,y)] == type;
}

void WorldMapCells::SetCellType(unsigned int x, unsigned int y, CellType type) {
	//UE_LOG(WorldGeneration, Log, TEXT("CellType for the Cell (%d, %d) is set to %d"), x, y, type);
	if (!IsCellInRange(x,y))
	{
		return;
	}
	
	Cells[D2ToD1(x, y)] = type;
}

void WorldMapCells::AddRoom(unsigned int pos, unsigned int size)
{
	RoomPos.Add(pos);
	RoomSize.Add(size);
	RoomType.Add(Empty);
}

const TArray<unsigned>* WorldMapCells::GetRoomPos() const
{
	return &RoomPos;
}

const TArray<unsigned>* WorldMapCells::GetRoomSize() const
{
	return &RoomSize;
}

const TArray<ERoomType>* WorldMapCells::GetRoomType() const
{
	return &RoomType;
}