// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PotionRecipe.generated.h"

UCLASS()
class UPotionRecipe : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TMap<FString, unsigned int> IngredientToQuantity;
	FString GetRecipeContent();
};
