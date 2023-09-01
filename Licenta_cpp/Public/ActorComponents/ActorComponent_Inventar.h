// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/InventarObject.h"
#include "Components/ActorComponent.h"
#include "ActorComponent_Inventar.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LICENTA_CPP_API UActorComponent_Inventar : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorComponent_Inventar();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static constexpr unsigned int Bar_Size = 10;
	int GetSize();

	UFUNCTION(BlueprintCallable)
	bool ConsumeUpdate();

	UFUNCTION(BlueprintCallable)
	int AddInventarObject(UInventarObject * Obj, bool AddToAccessBar = true);

	UFUNCTION(BlueprintCallable)
	int ModifyObjectQuantity(const FString& ObjName, int NewQuantity);

	UFUNCTION(BlueprintCallable)
	const TMap<FString, UInventarObject *> & GetContent();

	UFUNCTION(BlueprintCallable)
	const TArray<FString> & GetAccessBarContent();

	UFUNCTION(BlueprintCallable)
	int GetSelectedObjIdx();
	
	virtual int SelectAccessBarObject(const int ObjIdx);
	UInventarObject * GetSelectedObject();
	
	TMap<FString, UInventarObject * > Content;
	TArray<FString> AccessBarContent;

private:
	int SelectedObjIdx;
	bool NeedsUpdate;
};
