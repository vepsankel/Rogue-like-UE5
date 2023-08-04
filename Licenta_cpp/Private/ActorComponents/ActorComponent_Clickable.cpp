// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ActorComponent_Clickable.h"

#include "GameFramework/Actor.h"
#include "InputCoreTypes.h"

// Sets default values for this component's properties
UActorComponent_Clickable::UActorComponent_Clickable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UActorComponent_Clickable::Select(AActor* component, FKey key) {
	UE_LOG(LogTemp, Error, TEXT("UActorComponent_Clickable::Select() : clicked\r\n"));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("World delta for current frame equals %f"), GetWorld()->TimeSeconds));
}

// Called when the game starts
void UActorComponent_Clickable::BeginPlay()
{
	Super::BeginPlay();

	// ...

	AActor* owner = GetOwner();

	if (!owner) {
		UE_LOG(LogTemp, Warning, TEXT("UActorComponent_Clickable::BeginPlay() : owner is null\r\n"));
		return;
	}

	TArray<UStaticMeshComponent*> components;
	owner->GetComponents<UStaticMeshComponent>(components);

	if (components.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("UActorComponent_Clickable::BeginPlay() : owner is not null, but has no static meshes\r\n"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UActorComponent_Clickable::BeginPlay() : try to add select to owner\r\n"));
	owner->OnClicked.AddDynamic(this, &UActorComponent_Clickable::Select);
}

// Called every frame
void UActorComponent_Clickable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}