// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plant.generated.h"

UCLASS()
class LICENTA_CPP_API APlant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlant();
	
	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh *> PlantStages;

	UPROPERTY(EditAnywhere)
	TArray<unsigned int> PlantGrowthDuration;

	bool IsFinalStage(const int Stage) const;
	FString GetFullStageName(const int Stage) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
