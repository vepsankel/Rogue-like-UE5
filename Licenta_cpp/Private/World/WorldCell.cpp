// Fill out your copyright notice in the Description page of Project Settings.


#include "World/WorldCell.h"

// Sets default values
AWorldCell::AWorldCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->InitCell();
}

// Called when the game starts or when spawned
void AWorldCell::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWorldCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldCell::InitCell() {
	this->Type = CELLTYPE_EMPTY;
}

CellType AWorldCell::GetType() {
	return this->Type;
}

void AWorldCell::SetType(CellType type) {
	this->Type = type;
}

//void AWorldCell::equals(const AWorldCell& cell) {
//	this->Type = cell.Type;
//}