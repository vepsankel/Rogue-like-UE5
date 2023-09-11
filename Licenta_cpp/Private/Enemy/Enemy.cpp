// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LastTimeAttack = -AttackDuration;
	AttackProcessed = true;

	Hp = 100;
	ShouldBeDeleted = false;
}

bool AEnemy::GetIsAgroeg()
{
	return IsAgroed;
}

void AEnemy::SetIsAgroed(bool NewVal)
{
	IsAgroed = NewVal;
}

int AEnemy::GetHp()
{
	return Hp;
}

void AEnemy::SetHp(int Val, bool Relative)
{
	if (Relative)
	{
		Hp += Val;
	} else
	{
		Hp = Val;
	}

	if (Hp <= 0)
	{
		ShouldBeDeleted = true;
	}
}

void AEnemy::Attack(AActor* AttackTarget)
{
	LastTimeAttack = GetWorld()->GetTimeSeconds();
	this->Target = AttackTarget;
	StartAttackLoc = GetActorLocation();
	AttackProcessed = false;
}

float AEnemy::GetAttack1Phase()
{
	float AttackTime = GetWorld()->GetTimeSeconds() - LastTimeAttack;
	if (AttackTime < 0)
	{
		return 0;
	}

	if (AttackTime > Attack1Duration)
	{
		return 1;
	}

	return AttackTime / Attack1Duration;
}

float AEnemy::GetAttack2Phase()
{
	float AttackTime = GetWorld()->GetTimeSeconds() - LastTimeAttack - Attack1Duration;
	if (AttackTime < 0)
	{
		return 0;
	}

	if (AttackTime > Attack2Duration)
	{
		return 1;
	}

	return AttackTime / Attack2Duration;
}

float AEnemy::GetAttack3Phase()
{
	float AttackTime = GetWorld()->GetTimeSeconds() - LastTimeAttack - Attack1Duration - Attack2Duration;
	if (AttackTime < 0)
	{
		return 0;
	}

	if (AttackTime > Attack3Duration)
	{
		return 1;
	}

	return AttackTime / Attack3Duration;
}

float AEnemy::GetAttackPhase()
{
	float AttackTime = GetWorld()->GetTimeSeconds() - LastTimeAttack;
	if (AttackTime < 0)
	{
		return 0;
	}

	if (AttackTime > AttackDuration)
	{
		return 1;
	}

	return AttackTime / AttackDuration;
}

AActor* AEnemy::GetTarget()
{
	return Target;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldBeDeleted)
	{
		Destroy();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

