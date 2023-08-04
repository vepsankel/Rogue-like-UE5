// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputCoreTypes.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorComponent_Clickable.generated.h"

class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LICENTA_CPP_API UActorComponent_Clickable : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION()
	void Select(AActor* component, FKey fkey);

	// Sets default values for this component's properties
	UActorComponent_Clickable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
