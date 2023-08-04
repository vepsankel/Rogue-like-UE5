// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/Pawn_Main.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_TurnBased.generated.h"

/**
 * 
 */
UCLASS()
class LICENTA_CPP_API AGameMode_TurnBased : public AGameModeBase
{
	GENERATED_BODY()

	UClass* AGameMode_TurnBased::GetDefaultPawnClassForController(AController* InController);
};
