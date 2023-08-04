// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_Main.generated.h"

/**
 * 
 */
UCLASS()
class LICENTA_CPP_API APlayerState_Main : public APlayerState
{
	GENERATED_BODY()
	
private:
	APawn* PCInControl;
};
