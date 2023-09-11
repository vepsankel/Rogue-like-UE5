// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionRecipe.h"

FString UPotionRecipe::GetRecipeContent()
{
	FString Res = FString("Potion ingredients:");

	unsigned int i = 0;
	unsigned int size = IngredientToQuantity.Num();
	for (auto & IngredientTuple : IngredientToQuantity)
	{
		const FString IngredientName = IngredientTuple.Key;
		const unsigned int IngredientQuantity = IngredientTuple.Value;

		if (i != size)
		{
			Res.Append(IngredientName).Append(FString::Printf(TEXT(" (%u), "), IngredientQuantity));
		} else
		{
			Res.Append(IngredientName).Append(FString::Printf(TEXT(" (%u)"), IngredientQuantity));
		}
	}

	return Res;
}
