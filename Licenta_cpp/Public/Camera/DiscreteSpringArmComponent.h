// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "DiscreteSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class LICENTA_CPP_API UDiscreteSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	UDiscreteSpringArmComponent();
	void SetZoom(int32);
	void IncrementZoom(int32 increment = 1);
	void DecrementZoom(int32 decrement = 1);
	void Rotate(float dYaw);
	float GetYaw();
	int32 DefaultZoomLevel = 5;

private:
	void RecalculateSpringArm();
	const int32 MAX_ZOOM_LEVEL = 15;
	const int32 MIN_ZOOM_LEVEL = 1;
	int32 ZoomLevel;
	float dist, height, yaw, roll;
};
