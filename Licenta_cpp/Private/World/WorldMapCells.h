// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/WorldGlobals.h"
#include "World/WorldCell.h"
#include "World/WorldRoom.h"

/**
 * 
 */
class WorldMapCells
{
public:
	WorldMapCells(unsigned int Size);
	~WorldMapCells();

	void Initialise(unsigned int Size);
	unsigned int GetSize();
	
	int D2ToD1(unsigned int x, unsigned int y) const;
	void D1ToD2(unsigned int pos, unsigned int& x, unsigned int& y) const;

	void SetCellType(unsigned int x, unsigned int y, CellType type);

	bool IsCellInRange(unsigned int x, unsigned int y) const;
	bool IsCellOfType(unsigned int x, unsigned int y, CellType type) const;

	void AddRoom(unsigned int pos, unsigned int size);
	const TArray<unsigned int> * GetRoomPos() const;
	const TArray<unsigned int> * GetRoomSize() const;
	const TArray<ERoomType>* GetRoomType() const;
	
private:
	unsigned int Size;
	TArray<CellType> Cells;
	TArray<unsigned int> RoomPos;
	TArray<unsigned int> RoomSize;
	TArray<ERoomType> RoomType;
};
