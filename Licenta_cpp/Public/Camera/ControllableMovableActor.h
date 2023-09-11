// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "World/WorldMapCells.h"
#include "World/WorldMapGenerator.h"
#include "World/WorldGlobals.h"
#include "Potion/PotionRecipe.h"
#include "Potion/PotionProjectile.h"

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

	UFUNCTION(BlueprintCallable)
	bool GetCanReap();

	void DBG_WinEveryoneInRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	AWorldMapGenerator * WorldMap;

	// UPROPERTY(EditAnywhere)
	// APlant * SelectablePlant;
	
	UActorComponent_Inventar* GetInventarComponent();
	void SelectInventarObject(int);

	// Input
	void Action1();
	bool Action2(bool, const FRotator &);
	void Use();

	// Actions
	void AttackMelee();
	void Throw();
	void StartCrafting();
	void EndCrafting();
	void Plant();
	void Reap();

	// Auxilary
	void DrawAimingSpline();
	int MoveTo(FVector3f Dir, float DeltaTime, bool WithRotation = true);
	UPROPERTY(EditAnywhere);
	TSubclassOf<UStaticMesh> AimingSphere;
private:
	UPROPERTY()
	TArray<UStaticMesh*> AimingSpheres;
public:
	
	// Animation
	UFUNCTION(BlueprintCallable)
	bool GetIsMoving();

	UFUNCTION(BlueprintCallable)
	bool GetIsMeleeAttacking();

	UFUNCTION(BlueprintCallable)
	bool GetIsSeeding();

	UFUNCTION(BlueprintCallable)
	bool GetIsCrafting();

	UFUNCTION(BlueprintCallable)
	bool GetIsThrowing();

	// Animation speed
	UFUNCTION(BlueprintCallable)
	float GetSpeed();

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float NewSpeed);

private:
	bool IsSpawned;
	FVector3d FacingDir;
	FVector3d DesiredFacingDir;

	// Inventar
	UPROPERTY()
	UInventarObject * SelectedInventarObject;
	UPROPERTY()
	UActorComponent_Inventar * InventarComponent;
	
	// Crafting
	UPROPERTY()
	UPotionRecipe * PotionRecipe;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APotionProjectile> Projectile;
	
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

	// Ranged Attack
	bool IsAiming;
	bool IsThrowing;
	float LastTimeThrew;
	const float ThrowingDuration = 0.5;
	const float ThrowingDelay = 0.25;
	bool ThrowingDelayReached;
	FRotator AimingDir;

	// Crafting
	FString BulbPreviousDescription;
	bool IsCrafting;
	
	// UPROPERTY(EditAnywhere)
	bool CanSeed;
	void UpdateCanSeed();

	// Reaping
	bool CanReap;
	void UpdateCanReap();
};
