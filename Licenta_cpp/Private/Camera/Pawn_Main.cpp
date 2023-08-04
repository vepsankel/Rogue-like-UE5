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
APawn_Main::APawn_Main()
{
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

APawn_Main::APawn_Main(const FObjectInitializer& ObjectInitializer) : APawn_Main()
{
	// Add Camera
	{
		//UCapsuleComponent* capsuleComponent = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CapsuleComponent"));
		UCapsuleComponent* capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
		SetRootComponent(capsuleComponent);

		this->springArmComponent = CreateDefaultSubobject<UDiscreteSpringArmComponent>(TEXT("SpringArm"));
		this->springArmComponent->SetupAttachment(capsuleComponent);

		this->cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		this->cameraComponent->SetupAttachment(springArmComponent);

		this->sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Center"));
		this->sphereComponent->SetupAttachment(capsuleComponent);

		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::BeginPlay() : Added camera\r\n"));
	}
}

// Called when the game starts or when spawned
void APawn_Main::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("APAwn_Main::BeginPlay() : Entered function\r\n"));
	Super::BeginPlay();

	// Try to map input context to player controller
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (InputMapping) {
			UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			InputSystem->AddMappingContext(InputMapping, 0);
			UE_LOG(LogTemp, Log, TEXT("APAwn_Main::BeginPlay() : Added mapping context\r\n"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::BeginPlay() : InputMapping is null\r\n"));
		}

		PlayerController->SetShowMouseCursor(true);
	}

	

	SetActorLocation(FVector(0, 0, 20));
	UE_LOG(LogTemp, Log, TEXT("APAwn_Main::BeginPlay() : Exited function\r\n"));
}

// Called every frame
void APawn_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APawn_Main::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : Entered function\r\n"));

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (CameraRoamInputAction) {
		Input->BindAction(CameraRoamInputAction, ETriggerEvent::Triggered, this, &APawn_Main::Roam);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound CameraRoamInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : CameraRoamInputAction is null\r\n"));
	}

	if (CameraZoomInputAction) {
		Input->BindAction(CameraZoomInputAction, ETriggerEvent::Triggered, this, &APawn_Main::Zoom);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound CameraZoomInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : CameraZoomInputAction is null\r\n"));
	}

	if (CameraRotationInputAction) {
		Input->BindAction(CameraRotationInputAction, ETriggerEvent::Triggered, this, &APawn_Main::Rotate);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound CameraZoomInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : CameraZoomInputAction is null\r\n"));
	}

	UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : Exited function\r\n"));
}

void APawn_Main::Roam(const FInputActionInstance& Instance) {
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
}

void APawn_Main::Rotate(const FInputActionInstance& Instance) {
	float  mouseRotationInput = Instance.GetValue().Get<float>();

	if (!(this->springArmComponent)) {
		UE_LOG(LogTemp, Warning, TEXT("APawn_Main::Rotate() : SpringArmComponent is null"));
		return;
	}

	springArmComponent->Rotate(mouseRotationInput);
	SetActorRotation(FRotator(0, springArmComponent->GetYaw(), 0));
}

void APawn_Main::Zoom(const FInputActionInstance& Instance) {
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

