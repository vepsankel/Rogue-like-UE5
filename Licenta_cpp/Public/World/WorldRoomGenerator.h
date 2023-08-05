// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldCell.h"
#include "GameFramework/Actor.h"
#include "WorldRoomGenerator.generated.h"

UCLASS()
class LICENTA_CPP_API AWorldRoomGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldRoomGenerator();

	int GenerateRooms(WorldMapCells * MapCells);
	
	UPROPERTY(EditAnywhere)
		unsigned int RoomSizes[10];

	UPROPERTY(EditAnywhere)
		unsigned int RoomDistances[10];

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	WorldMapCells * Cells;
};
