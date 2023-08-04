// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_TurnBased.h"
#include "Camera/Pawn_Main.h"

UClass* AGameMode_TurnBased::GetDefaultPawnClassForController(AController* InController) {
	return APawn_Main::StaticClass();
};