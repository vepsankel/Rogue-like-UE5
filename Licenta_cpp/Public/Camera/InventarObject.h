// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Plant/Plant.h"
#include "InventarObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LICENTA_CPP_API UInventarObject : public UObject
{
	GENERATED_BODY()
public:
	UInventarObject();

	UPROPERTY(EditAnywhere)
	FString ObjectName;

	UPROPERTY(EditAnywhere)
	FString HumanReadableName;

	UPROPERTY(EditAnywhere)
	bool HasQuantity;
	
	UPROPERTY(EditAnywhere)
	int Quantity;

	UPROPERTY(EditAnywhere)
	bool IsIngridient;

	UPROPERTY(EditAnywhere)
	bool IsMeleeWeapon;

	UPROPERTY(EditAnywhere)
	bool IsThrowable;

	UPROPERTY(EditAnywhere)
	bool IsUsable;

	UPROPERTY(EditAnywhere)
	APlant * Plant;

	UPROPERTY(EditAnywhere)
	UTexture* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HoldableComponent;

	UPROPERTY(EditAnywhere)
	FString ItemDescription;

	UFUNCTION(BlueprintCallable)
	FString GetObjectName() const;
	UFUNCTION(BlueprintCallable)
	void SetObjectName(const FString& NewObjectName);

	UFUNCTION(BlueprintCallable)
	FString GetHumanReadableName() const;
	UFUNCTION(BlueprintCallable)
	void SetHumanReadableName(const FString& NewHumanReadableName);

	UFUNCTION(BlueprintCallable)
	FString GetItemDescription() const;
	UFUNCTION(BlueprintCallable)
	void SetItemDescription(const FString& NewItemDescription);

	UFUNCTION(BlueprintCallable)
	bool GetHasQuantity() const;
	UFUNCTION(BlueprintCallable)
	void SetHasQuantity(bool bHasQuantity);

	UFUNCTION(BlueprintCallable)
	int GetQuantity() const;
	UFUNCTION(BlueprintCallable)
	void SetQuantity(int NewQuantity);

	UFUNCTION(BlueprintCallable)
	bool GetIsIngridient() const;
	UFUNCTION(BlueprintCallable)
	void SetIsIngridient(bool bIsIngridient);

	UFUNCTION(BlueprintCallable)
	UTexture* GetIcon() const;
	UFUNCTION(BlueprintCallable)
	void SetIcon(UTexture* NewIcon);

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetHoldableComponent() const;
	UFUNCTION(BlueprintCallable)
	void SetHoldableComponent(UStaticMeshComponent* NewHoldableComponent);

	UFUNCTION(BlueprintCallable)
	UInventarObject* BuildObject(
		FString NewObjectName,
		FString NewHumanReadableName,
		FString NewItemDescription,
		bool NewHasQuantity,
		int NewQuantity,
		bool NewIsMeleeWeapon,
		bool NewIsThrowable,
		bool NewIsUsable,
		bool NewIsIngridient,
		APlant * Plantable,
		UTexture* NewIcon,
		UStaticMeshComponent* NewHoldableComponent);
};
