#pragma once

#include "InputAction.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pawn_Main.generated.h"

//class UEnhancedInputComponent;
class UInputAction;
class UCameraComponent;
class UDiscreteSpringArmComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class LICENTA_CPP_API APawn_Main : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawn_Main();
	APawn_Main(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, Category = "Enhanced Input")
	UCameraComponent* cameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDiscreteSpringArmComponent* springArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Enhanced Input")
	USphereComponent* sphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
