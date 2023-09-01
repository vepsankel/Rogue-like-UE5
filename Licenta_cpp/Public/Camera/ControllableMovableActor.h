// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "World/WorldMapCells.h"
#include "World/WorldMapGenerator.h"
#include "World/WorldGlobals.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorComponents/ActorComponent_Inventar.h"
#include "ControllableMovableActor.generated.h"

UCLASS()
class LICENTA_CPP_API AControllableMovableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AControllableMovableActor();

	UFUNCTION(BlueprintCallable)
	bool GetCanSeed();

	void DBG_WinEveryoneInRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int MoveTo(FVector3f Dir, float DeltaTime, bool WithRotation = true);

	UPROPERTY(EditAnywhere)
	AWorldMapGenerator * WorldMap;

	// UPROPERTY(EditAnywhere)
	// APlant * SelectablePlant;
	
	const UActorComponent_Inventar* GetInventarComponent();
	void Plant(APlant * SelectedPlant);
	void SelectInventarObject(int);

	// Input
	void Action1();
	void Action2();
	void Use();

	// Actions
	void AttackMelee();
	void StartCrafting();

	// Animation
	UFUNCTION(BlueprintCallable)
	bool GetIsMoving();

	UFUNCTION(BlueprintCallable)
	bool GetIsMeleeAttacking();

	UFUNCTION(BlueprintCallable)
	bool GetIsSeeding();

	UFUNCTION(BlueprintCallable)
	bool GetIsCrafting();

	// Animation speed
	UFUNCTION(BlueprintCallable)
	float GetSpeed();

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float NewSpeed);

private:
	FVector3d FacingDir;
	FVector3d DesiredFacingDir;
	
	// Animation properties

	// Movement
	float LastTimeMoved;
	bool IsMoving;
	const float BaseSpeed = 1000;
	float Speed = BaseSpeed;

	// Seeding plants;
	float LastTimeSeed;
	const float PlantingDuration = 1;
	bool IsPlanting;

	// Melee Attack
	float LastTimeAttacked;
	const float MeleeAttackDuration = 0.8;
	bool IsAttacking;

	// Crafting
	bool IsCrafting;
	// UPROPERTY(EditAnywhere)
	bool CanSeed;
};
