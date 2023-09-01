// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	ERoomType RoomType;
};
