// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_HUD.generated.h"

/**
 * 
 */
UCLASS()
class LICENTA_CPP_API UUserWidget_HUD : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere);
	UUserWidget* SelectedObjectDetailsMenu;

	UPROPERTY(EditAnywhere);
	UUserWidget* ActionBar;


};
