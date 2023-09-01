#include "Camera/Pawn_Main.h"
#include "Camera/DiscreteSpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include <EnhancedInputSubsystems.h>
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APawn_Main::APawn_Main()
{
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HP = 100;
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

	// Get Inventar
	if (this->ControllableMovableActor != nullptr)
	{
		this->InventarComponent = ControllableMovableActor->GetInventarComponent();
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

	// CanSeed = false;
	
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

	if (PlayablePlantGatherInputAction) {
		Input->BindAction(PlayablePlantGatherInputAction, ETriggerEvent::Triggered, this, &APawn_Main::Plant);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound PlayablePlantGatherInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : PlayablePlantGatherInputAction is null\r\n"));
	}

	if (ToggleInventarInputAction) {
		Input->BindAction(ToggleInventarInputAction, ETriggerEvent::Triggered, this, &APawn_Main::Inventar);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound ToggleInventarInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : ToggleInventarInputAction is null\r\n"));
	}

	if (DebugSkipRoomInputAction)
	{
		Input->BindAction(DebugSkipRoomInputAction, ETriggerEvent::Triggered, this, &APawn_Main::DBG_SkipRoom);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound DebugSkipRoomInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : DebugSkipRoomInputAction is null\r\n"));
	}

	if (SelectInventarObjectInputAction)
	{
		Input->BindAction(SelectInventarObjectInputAction, ETriggerEvent::Triggered, this, &APawn_Main::SelectInventarObject);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound SelectInventarObjectInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : SelectInventarObjectInputAction is null\r\n"));
	}

	if (Action1InputAction)
	{
		Input->BindAction(Action1InputAction, ETriggerEvent::Triggered, this, &APawn_Main::Action1);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound Action1InputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : Action1InputAction is null\r\n"));
	}

	if (Action2InputAction)
	{
		Input->BindAction(Action2InputAction, ETriggerEvent::Triggered, this, &APawn_Main::Action2);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound Action2InputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : Action2InputAction is null\r\n"));
	}

	if (UseInputAction)
	{
		Input->BindAction(UseInputAction, ETriggerEvent::Triggered, this, &APawn_Main::Use);
		UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : bound UseInputAction\r\n"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("APAwn_Main::SetupPlayerInputComponent() : UseInputAction is null\r\n"));
	}
	
	UE_LOG(LogTemp, Log, TEXT("APAwn_Main::SetupPlayerInputComponent() : Exited function\r\n"));
}

void APawn_Main::Roam(const FInputActionInstance& Instance) {
	FVector2d mouseRoamInput = Instance.GetValue().Get<FVector2d>();
	static float LastProcessedRoamTime = 0;
	const float ElapsedTime = Instance.GetElapsedTime();

	if (ElapsedTime < LastProcessedRoamTime)
	{
		LastProcessedRoamTime = 0;
	}

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

	if (ControllableMovableActor != nullptr)
	{
		ControllableMovableActor->MoveTo(FVector3f(camDir.X, camDir.Y, camDir.Z), ElapsedTime - LastProcessedRoamTime);
		SetActorLocation(ControllableMovableActor->GetActorLocation());
	} else
	{
		camDir *= 10;
		SetActorLocation(loc + camDir);
	}

	LastProcessedRoamTime = ElapsedTime;
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

	HP--;
}

void APawn_Main::Plant(const FInputActionInstance& Instance)
{
	// ControllableMovableActor->Plant();
}

void APawn_Main::DBG_SkipRoom(const FInputActionInstance& Instance)
{
	// Room skip
	ControllableMovableActor->DBG_WinEveryoneInRoom();
}

void APawn_Main::Inventar(const FInputActionInstance& Instance)
{
	// Inventar???
}

void APawn_Main::SelectInventarObject(const FInputActionInstance& Instance)
{
	FVector2d Press = Instance.GetValue().Get<FVector2d>();
	
	ControllableMovableActor->SelectInventarObject(Press.X);
}

void APawn_Main::Action1(const FInputActionInstance& Instance)
{
	ControllableMovableActor->Action1();
}

void APawn_Main::Action2(const FInputActionInstance& Instance)
{
	ControllableMovableActor->Action2();
}

void APawn_Main::Use(const FInputActionInstance& Instance)
{
	ControllableMovableActor->Use();
}

int APawn_Main::GetHp() const
{
	return HP;
}

