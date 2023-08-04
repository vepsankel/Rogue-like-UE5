// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/DiscreteSpringArmComponent.h"

UDiscreteSpringArmComponent::UDiscreteSpringArmComponent() {
}

void UDiscreteSpringArmComponent::BeginPlay() {
	Super::BeginPlay();

	this->ZoomLevel = this->DefaultZoomLevel;
	bEnableCameraLag = true;
	bEnableCameraRotationLag = true;
	this->yaw = 0;
	RecalculateSpringArm();
}

void UDiscreteSpringArmComponent::IncrementZoom(int32 increment) {
	if (ZoomLevel >= MAX_ZOOM_LEVEL)
		return;

	SetZoom(ZoomLevel + increment);
}

void UDiscreteSpringArmComponent::DecrementZoom(int32 decrement) {
	if (ZoomLevel <= MIN_ZOOM_LEVEL)
		return;

	SetZoom(ZoomLevel - decrement);
}

void UDiscreteSpringArmComponent::SetZoom(int32 Zoom) {
	Zoom = (Zoom > MAX_ZOOM_LEVEL) ? MAX_ZOOM_LEVEL : (Zoom < MIN_ZOOM_LEVEL ? MIN_ZOOM_LEVEL : Zoom);
	
	if (Zoom == ZoomLevel)
		return;

	this->ZoomLevel = Zoom;
	RecalculateSpringArm();
}

void UDiscreteSpringArmComponent::Rotate(float dYaw) {
	this->yaw += dYaw;
	RecalculateSpringArm();
}

float UDiscreteSpringArmComponent::GetYaw() {
	return this->yaw;
}

void UDiscreteSpringArmComponent::RecalculateSpringArm() {
	dist = ZoomLevel * 40 + 25;
	height = ZoomLevel * ZoomLevel * 4;
	roll = -(ZoomLevel - MIN_ZOOM_LEVEL) * 4;

	//TargetArmLength = dist;
	TargetOffset = FVector3d(-dist, 0, height).RotateAngleAxis(yaw, FVector3d::UpVector);
	//TargetOffset = FVector(-dist,0,height);
	SetRelativeRotation(FRotator(roll,0,0));
}