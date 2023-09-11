// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class LICENTA_CPP_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Agro
	UFUNCTION(BlueprintCallable)
	bool GetIsAgroeg();
	UFUNCTION(BlueprintCallable)
	virtual void SetIsAgroed(bool NewVal);

	// HP
	UFUNCTION(BlueprintCallable)
	int GetHp();
	UFUNCTION(BlueprintCallable)
	virtual void SetHp(int Val, bool Relative = false);

	// Attack
	UFUNCTION(BlueprintCallable)
	virtual void Attack(AActor * Target);
	float GetAttack1Phase();
	float GetAttack2Phase();
	float GetAttack3Phase();
	float GetAttackPhase();
	UFUNCTION(BlueprintCallable)
	AActor* GetTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	int Hp;
	bool IsAgroed;

	float LastTimeAttack;
	
	float Attack1Duration = 1;
	float Attack2Duration = 1;
	float Attack3Duration = 1;
	
	float AttackDuration = Attack1Duration + Attack2Duration + Attack3Duration;

protected:
	bool ShouldBeDeleted;
	AActor * Target;
	FVector StartAttackLoc;
	bool AttackProcessed;
};
