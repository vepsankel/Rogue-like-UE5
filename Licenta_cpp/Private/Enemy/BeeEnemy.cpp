// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BeeEnemy.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "World/WorldGlobals.h"

void ABeeEnemy::Attack(AActor* AttackTarget)
{
	Super::Attack(AttackTarget);

	SkelMesh = (USkeletalMeshComponent *)GetComponentByClass(USkeletalMeshComponent::StaticClass());
	// SkelMesh->SetRelativeLocation(FVector(0,0,0), true);
	CollisionComponent = (UCapsuleComponent*)GetComponentByClass(UCapsuleComponent::StaticClass());
}

void ABeeEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float AttackPhase = GetAttackPhase();

	FVector FlyingLocation(0,0,400);
	FVector WalkingLocation(0,0,50);
	FVector VerticalLocation;
	FVector HorizontalLocation;
	FVector TargetLocation;
	
	if (Target != nullptr)
		TargetLocation = Target->GetActorLocation() - StartAttackLoc;

	float HorizontalKPhase1 = 0.1;

	if (!AttackProcessed && AttackPhase > 0.01 && AttackPhase < 0.99)
	{
		if (CollisionComponent != nullptr)
		{
			CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Block);
		}
		
		float Attack1Phase = GetAttack1Phase();
		float Attack2Phase = GetAttack2Phase();
		float Attack3Phase = GetAttack3Phase();

		VerticalLocation = WalkingLocation + (FlyingLocation - (FlyingLocation - WalkingLocation) * Attack1Phase + FlyingLocation * Attack3Phase);
			//(WalkingLocation - FlyingLocation) * Attack1Phase + FlyingLocation + WalkingLocation + (FlyingLocation - WalkingLocation) * Attack3Phase;
		HorizontalLocation = StartAttackLoc + TargetLocation *
			(Attack1Phase * HorizontalKPhase1 +
			(1 - HorizontalKPhase1) * Attack2Phase)- Attack3Phase/2;

		// HorizontalLocation = TargetLocation + StartAttackLoc;
	} else {
		if (!AttackProcessed && AttackPhase > 0.5)
		{
			StartAttackLoc = GetActorLocation();

			// SetActorRotation(TargetLocation.Rotation());
			SetActorLocation(Target->GetActorLocation(), true);

			AttackProcessed = true;

			if (CollisionComponent != nullptr)
			{
				CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Ignore);
			}
		
			// if (SkelMesh != nullptr)
			// 	SkelMesh->SetRelativeLocation(VerticalLocation);
			return;
		}

		if (SkelMesh != nullptr)
		{
			SkelMesh->SetRelativeRotation(FVector(1,0,0).Rotation());
		}
		return;
	}
	
	SetActorLocation(HorizontalLocation, false);
	
	if (SkelMesh != nullptr)
	{
		SkelMesh->SetRelativeLocation(VerticalLocation);
		SkelMesh->SetWorldRotation(FVector(TargetLocation.X, TargetLocation.Y, 0).Rotation());
	}
}
