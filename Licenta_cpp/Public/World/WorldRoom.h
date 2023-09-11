// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "DrawDebugHelpers.h"
#include "WorldRoom.generated.h"

enum ERoomType
{
	Empty,
	Enemy_Bee,
	Obstacle_Pond
};

/**
 * 
 */
UCLASS()
class LICENTA_CPP_API AWorldRoom : public ATriggerBox
{
	GENERATED_BODY()
public:
	AWorldRoom();

	virtual void BeginPlay() override;

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> Enemy;

	ERoomType RoomType;
};
