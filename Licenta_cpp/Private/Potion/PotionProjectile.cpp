// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion/PotionProjectile.h"

// Sets default values
APotionProjectile::APotionProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APotionProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APotionProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

