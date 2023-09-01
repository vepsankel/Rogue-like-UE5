 #pragma once

#include "InputAction.h"
#include "Camera/ControllableMovableActor.h"

#include "CoreMinimal.h"
#include "ActorComponents/ActorComponent_Inventar.h"
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

	// Input Action used to tell controllable actor to talk with plants
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* PlayablePlantGatherInputAction;

	// Input Action used to emulate room skipping
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* DebugSkipRoomInputAction;

	// Input Action used to toggle inventar
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* ToggleInventarInputAction;

	// Input Action used to select inventar object
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* SelectInventarObjectInputAction;

	// Input Action used for action 1
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* Action1InputAction;

	// Input Action used for action 2
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* Action2InputAction;

	// Input Action used to use inventar object
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputAction* UseInputAction;

	// Controllable Component
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	AControllableMovableActor* ControllableMovableActor;
	
	// Handle triggered by CameraRoamInputAction.
	// Used to move camera around
	virtual void Roam(const FInputActionInstance& Instance);

	// Handle triggered by CameraZoomInputAction.
	// Used to zoom camera
	virtual void Zoom(const FInputActionInstance& Instance);

	// Handle triggered by CameraZoomInputAction.
	// Used to zoom camera
	virtual void Rotate(const FInputActionInstance& Instance);
	
	virtual void Plant(const FInputActionInstance& Instance);

	virtual void DBG_SkipRoom(const FInputActionInstance& Instance);

	virtual void Inventar(const FInputActionInstance& Instance);

	virtual void SelectInventarObject(const FInputActionInstance& Instance);

	virtual void Action1(const FInputActionInstance& Instance);

	virtual void Action2(const FInputActionInstance& Instance);

	virtual void Use(const FInputActionInstance& Instance);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Player parameters, usable for HUD
	UFUNCTION(BlueprintCallable)
	int GetHp() const;

	UPROPERTY(EditAnywhere)
	const UActorComponent_Inventar * InventarComponent;

private:
	int HP;
	// bool CanSeed;
};
