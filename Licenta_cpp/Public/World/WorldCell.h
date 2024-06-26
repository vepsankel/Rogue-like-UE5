// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "WorldCell.generated.h"

enum CellType {
	CELLTYPE_OBSTACLE,
	CELLTYPE_EMPTY,
	CELLTYPE_SOIL_NO_PLANT,
	CELLTYPE_SOIL_PLANT,
	CELLTYPE_DEBUG
};

UCLASS()
class LICENTA_CPP_API AWorldCell : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldCell();
	void InitCell();
	CellType GetType();
	void SetType(CellType type);
	//void equals(const AWorldCell&);
	CellType Type;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
