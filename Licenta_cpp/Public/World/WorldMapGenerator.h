// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <algorithm>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

#include "WorldRoomGenerator.h"
#include "World/WorldCell.h"
#include "World/WorldGlobals.h"
#include "World/WorldRandom.h"
#include "World/WorldMapCells.h"
#include "World/WorldMazeGenerator.h"
#include "WorldMapGenerator.generated.h"

UCLASS()
class LICENTA_CPP_API AWorldMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldMapGenerator();
	
	void SetSeed(unsigned int seed);
	
	int GenerateMap();
	int BuildMap();

	UPROPERTY(EditAnywhere)
		int Size;

	UPROPERTY(EditAnywhere)
		unsigned int Seed;

	UPROPERTY(EditAnywhere)
		AStaticMeshActor* WallStaticMeshActor;

	UPROPERTY(EditAnywhere)
		AWorldMazeGenerator* MazeGenerator;

	UPROPERTY(EditAnywhere)
		AWorldRoomGenerator* RoomGenerator;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	WorldMapCells Cells = WorldMapCells(Size);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};