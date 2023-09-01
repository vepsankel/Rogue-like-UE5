// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "Camera/Pawn_Main.h"
#include "Camera/DiscreteSpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include <EnhancedInputSubsystems.h>
#include "Camera/CameraActor.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// // Add Camera
	// {
	// 	//UCapsuleComponent* capsuleComponent = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CapsuleComponent"));
	// 	UCapsuleComponent* capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	// 	SetRootComponent(capsuleComponent);
	//
	// 	this->springArmComponent = CreateDefaultSubobject<UDiscreteSpringArmComponent>(TEXT("SpringArm"));
	// 	this->springArmComponent->SetupAttachment(capsuleComponent);
	//
	// 	this->cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// 	this->cameraComponent->SetupAttachment(springArmComponent);
	//
	// 	this->sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Center"));
	// 	this->sphereComponent->SetupAttachment(capsuleComponent);
	//
	// 	UE_LOG(LogTemp, Log, TEXT("APAwn_Main::BeginPlay() : Added camera\r\n"));
	// }

	// UE_LOG(LogTemp, Error, TEXT("OLOLOLO"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : Entered function\r\n"));
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (CameraRoamInputAction) {
		Input->BindAction(CameraRoamInputAction, ETriggerEvent::Triggered, this, &AMyCharacter::Roam);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound CameraRoamInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : CameraRoamInputAction is null\r\n"));
	}

	if (CameraZoomInputAction) {
		Input->BindAction(CameraZoomInputAction, ETriggerEvent::Triggered, this, &AMyCharacter::Zoom);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound CameraZoomInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : CameraZoomInputAction is null\r\n"));
	}

	if (CameraRotationInputAction) {
		Input->BindAction(CameraRotationInputAction, ETriggerEvent::Triggered, this, &AMyCharacter::Rotate);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound CameraZoomInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : CameraZoomInputAction is null\r\n"));
	}

	UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : Exited function\r\n"));
}

void AMyCharacter::Roam(const FInputActionInstance& Instance) {
	FVector2d mouseRoamInput = Instance.GetValue().Get<FVector2d>();

	if (!(this->cameraComponent)) {
		UE_LOG(LogTemp, Warning, TEXT("APawn_Main::Roam() : CameraComponent is null"));
		return;
	}

	if (!(this->springArmComponent)) {
		UE_LOG(LogTemp, Warning, TEXT("APawn_Main::Roam() : springArmComponent is null"));
		return;
	}

	FVector3d loc = GetActorLocation();
	//FVector3d camLoc = this->cameraComponent->GetComponentLocation() - loc;
	FVector3d camLoc = -FVector3d::ForwardVector.RotateAngleAxis(this->springArmComponent->GetYaw(), FVector3d::UpVector);
	FVector3d fw = -FVector3d(camLoc.X, camLoc.Y, 0);
	FVector3d r = FVector::CrossProduct(fw, FVector3d::UpVector);
	r.Normalize(0.01f);
	fw.Normalize(0.01f);

	FVector3d camDir = fw * mouseRoamInput.X + r * mouseRoamInput.Y;
	camDir.Normalize(0.01f);
	camDir *= 30;

	SetActorLocation(loc + camDir);
	UE_LOG(LogTemp, Error, TEXT("OLOLOLO"));
}

void AMyCharacter::Rotate(const FInputActionInstance& Instance) {
	float  mouseRotationInput = Instance.GetValue().Get<float>();

	if (!(this->springArmComponent)) {
		UE_LOG(LogTemp, Warning, TEXT("APawn_Main::Rotate() : SpringArmComponent is null"));
		return;
	}

	springArmComponent->Rotate(mouseRotationInput);
	SetActorRotation(FRotator(0, springArmComponent->GetYaw(), 0));
}

void AMyCharacter::Zoom(const FInputActionInstance& Instance) {
	int32 mouseZoom = -(int32)Instance.GetValue().Get<float>();

	if (springArmComponent) {

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APawn_Main::Zoom(const FInputActionInstance& Instance) : springArmComponent is null\r\n"));
		return;
	}

	if (mouseZoom > 0) {
		springArmComponent->IncrementZoom(mouseZoom);
	}
	else if (mouseZoom < 0) {
		springArmComponent->DecrementZoom(-mouseZoom);
	}
}