// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "BeeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class LICENTA_CPP_API ABeeEnemy : public AEnemy
{
	GENERATED_BODY()

	virtual void Attack(AActor* Target) override;

	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
	USkeletalMeshComponent * SkelMesh;

	UPROPERTY()
	UCapsuleComponent * CollisionComponent;
};
