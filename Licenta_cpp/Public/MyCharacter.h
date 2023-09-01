// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputAction.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

//class UEnhancedInputComponent;
class UInputAction;
class UCameraComponent;
class UDiscreteSpringArmComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class LICENTA_CPP_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(VisibleAnywhere, Category = "Enhanced Input")
	UCameraComponent* cameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDiscreteSpringArmComponent* springArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Enhanced Input")
	USphereComponent* sphereComponent;

	// Mapping Context
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputMappingContext* InputMapping;

	// Input Action used to roam the camera around
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* CameraRoamInputAction;

	// Input Action used to zoom the camera
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* CameraZoomInputAction;

	// Input Action used to zoom the camera
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* CameraRotationInputAction;

	// Handle triggered by CameraRoamInputAction.
	// Used to move camera around
	virtual void Roam(const FInputActionInstance& Instance);

	// Handle triggered by CameraZoomInputAction.
	// Used to zoom camera
	virtual void Zoom(const FInputActionInstance& Instance);

	// Handle triggered by CameraZoomInputAction.
	// Used to zoom camera
	virtual void Rotate(const FInputActionInstance& Instance);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
