// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponents/ActorComponent_Inventar.h"

#include "World/WorldGlobals.h"
#include "WorldPartition/ContentBundle/ContentBundleStatus.h"

// Sets default values for this component's properties
UActorComponent_Inventar::UActorComponent_Inventar()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorComponent_Inventar::BeginPlay()
{
	Super::BeginPlay();

	AccessBarContent.Init("", Bar_Size);
}

int UActorComponent_Inventar::GetSize()
{
	return Bar_Size;
}

bool UActorComponent_Inventar::ConsumeUpdate()
{
	if (NeedsUpdate)
	{
		NeedsUpdate = false;
		return true;
	}

	return false;
}

int UActorComponent_Inventar::AddInventarObject(UInventarObject * Obj, bool AddToAccessBar)
{
	Content.Add(Obj->GetObjectName(), Obj);
	UE_LOG(Player, Log, TEXT("Added Inventar Object %s"), *FString(Obj->GetObjectName()));

	unsigned int i = 0;
	if (AddToAccessBar)
	{
		for (auto & Name : AccessBarContent)
		{
			if (Name.Equals(""))
			{
				Name = Obj->GetObjectName();
				UE_LOG(Player, Log, TEXT("For object %s there is a slot nr.%d"), *FString(Obj->GetObjectName()), i++);
				NeedsUpdate = true;
				break;
			}
		}
	}

	NeedsUpdate = true;
	return WORLD_SUCCESS;
}

int UActorComponent_Inventar::ModifyObjectQuantity(const FString &ObjName, int Quantity, bool Relative)
{
	UInventarObject ** PObj = Content.Find(ObjName);
	UInventarObject * Obj = nullptr;

	if (PObj != nullptr)
	{
		Obj = *PObj;
	}
	
	if (Obj == nullptr)
	{
		UE_LOG(Player, Error, TEXT("Cannot modify quantity of object %s: object does not exist"), *FString(ObjName));
		return WORLD_FAILURE;
	}

	if (!Obj->GetHasQuantity())
	{
		UE_LOG(Player, Warning, TEXT("Cannot modify quantity of object %s: object is uncountable"), *FString(ObjName));
		return WORLD_FAILURE;
	}

	const int OldQuantity = Obj->GetQuantity();

	if (Relative)
	{
		Obj->SetQuantity(Quantity + OldQuantity);
	} else
	{
		Obj->SetQuantity(Quantity);
	}

	if (Obj->GetQuantity() <= 0)
	{
		for (auto & ObjectName : AccessBarContent)
		{
			if (ObjectName.Equals(ObjName))
			{
				ObjectName = "";
			}
		}
	} else if (OldQuantity == 0 )
	{
		for (auto & ObjectName : AccessBarContent)
		{
			if (ObjectName.Equals(""))
			{
				ObjectName = ObjName;
				break;
			}
		}
	}

	
	
	NeedsUpdate = true;
	return WORLD_SUCCESS;
}

int UActorComponent_Inventar::ModifyObjectDescription(const FString& ObjName, FString ObjDescription)
{
	UInventarObject ** PObj = Content.Find(ObjName);
	UInventarObject * Obj = nullptr;

	if (PObj != nullptr)
	{
		Obj = *PObj;
	}
	
	if (Obj == nullptr)
	{
		UE_LOG(Player, Error, TEXT("Cannot modify quantity of object %s: object does not exist"), *FString(ObjName));
		return WORLD_FAILURE;
	}

	NeedsUpdate = true;
	Obj->SetItemDescription(ObjDescription);
	return WORLD_SUCCESS;
}

const TMap<FString, UInventarObject*> & UActorComponent_Inventar::GetContent()
{
	return Content;
}

const TArray<FString> & UActorComponent_Inventar::GetAccessBarContent()
{
	return AccessBarContent;
}

int UActorComponent_Inventar::GetSelectedObjIdx()
{
	return SelectedObjIdx;
}

int UActorComponent_Inventar::SelectAccessBarObject(const int ObjIdx)
{
	if (ObjIdx >= 10 || ObjIdx < -1)
	{
		UE_LOG(Player, Error, TEXT("Player selected object %d, but it is out of range"), ObjIdx);
		return WORLD_FAILURE;
	}

	if (ObjIdx != SelectedObjIdx)
	{
		this->SelectedObjIdx = ObjIdx;
		this->NeedsUpdate = true;
	}
	
	return WORLD_SUCCESS;
}

UInventarObject* UActorComponent_Inventar::GetSelectedObject()
{
	if (SelectedObjIdx < 0 || SelectedObjIdx > 9)
		return nullptr;

	FString SelectedObjName = AccessBarContent[SelectedObjIdx];
	UInventarObject ** PSelectedObject = Content.Find(SelectedObjName);
	
	return (PSelectedObject == nullptr) ? nullptr : *PSelectedObject;
}

UInventarObject* UActorComponent_Inventar::GetObjectByIdx(const int ObjIdx)
{
	const FString ObjName = AccessBarContent[ObjIdx];

	if (ObjName.Equals(""))
	{
		return nullptr;
	}

	UInventarObject ** PObj = Content.Find(ObjName);

	if (PObj == nullptr)
	{
		return nullptr;
	}
	
	return *PObj;
}

// Called every frame
void UActorComponent_Inventar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

