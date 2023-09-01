// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/InventarObject.h"

#include "Plant/Plant.h"

UInventarObject::UInventarObject()
{
}

FString UInventarObject::GetObjectName() const
{
	return ObjectName;
}

void UInventarObject::SetObjectName(const FString& NewObjectName)
{
	this->ObjectName = NewObjectName;
}

FString UInventarObject::GetHumanReadableName() const
{
	return HumanReadableName;
}

void UInventarObject::SetHumanReadableName(const FString& NewHumanReadableName)
{
	this->HumanReadableName = NewHumanReadableName;
}

FString UInventarObject::GetItemDescription() const
{
	return this->ItemDescription;
}

void UInventarObject::SetItemDescription(const FString& NewItemDescription)
{
	this->ItemDescription = NewItemDescription;
}

bool UInventarObject::GetHasQuantity() const
{
	return HasQuantity;
}

void UInventarObject::SetHasQuantity(bool bHasQuantity)
{
	HasQuantity = bHasQuantity;
}

int UInventarObject::GetQuantity() const
{
	return Quantity;
}

void UInventarObject::SetQuantity(int NewQuantity)
{
	this->Quantity = NewQuantity;
}

bool UInventarObject::GetIsIngridient() const
{
	return IsIngridient;
}

void UInventarObject::SetIsIngridient(bool bIsIngridient)
{
	IsIngridient = bIsIngridient;
}

UTexture* UInventarObject::GetIcon() const
{
	return Icon;
}

void UInventarObject::SetIcon(UTexture* NewIcon)
{
	this->Icon = NewIcon;
}

UStaticMeshComponent* UInventarObject::GetHoldableComponent() const
{
	return HoldableComponent;
}

void UInventarObject::SetHoldableComponent(UStaticMeshComponent* NewHoldableComponent)
{
	HoldableComponent = NewHoldableComponent;
}

UInventarObject* UInventarObject::BuildObject(
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
	
	UStaticMeshComponent* NewHoldableComponent)
{
	this->SetObjectName(NewObjectName);
	this->SetHumanReadableName(NewHumanReadableName);
	this->ItemDescription = NewItemDescription;
	this->SetHasQuantity(NewHasQuantity);
	this->SetQuantity(NewQuantity);
	this->SetIsIngridient(NewIsIngridient);
	this->SetIcon(NewIcon);
	this->HoldableComponent = NewHoldableComponent;
	this->IsMeleeWeapon = NewIsMeleeWeapon;
	this->IsThrowable = NewIsThrowable;
	this->IsUsable = NewIsUsable;
	this->Plant = Plantable;
	return this;
}
