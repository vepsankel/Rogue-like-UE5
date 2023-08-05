// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/WorldCell.h"
#include "World/WorldMapCells.h"
#include "GameFramework/Actor.h"
#include "WorldRoomGenerator.generated.h"

#define NO_ROOM UINT_MAX
#define MAX_ROOM_TYPES 10

UCLASS()
class LICENTA_CPP_API AWorldRoomGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldRoomGenerator();

	int GenerateRooms(WorldMapCells * MapCells);
	
	UPROPERTY(EditAnywhere)
		unsigned int RoomSizes[MAX_ROOM_TYPES];

	UPROPERTY(EditAnywhere)
		unsigned int RoomAmount[MAX_ROOM_TYPES];

	UPROPERTY(EditAnywhere, meta=(UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 100))
		unsigned int RoomExpansionChancePercent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	unsigned int GenerationPointsAvailable[MAX_ROOM_TYPES];
	WorldMapCells * Cells;
	TArray<unsigned int> Rooms;
};
