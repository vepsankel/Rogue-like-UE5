// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/ControllableMovableActor.h"

#include "AnimationCompression.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"

// Sets default values
AControllableMovableActor::AControllableMovableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LastTimeMoved = 0;
	IsMoving = false;
	
	IsAttacking = false;
	LastTimeAttacked = -MeleeAttackDuration - 1;
}

// Called when the game starts or when spawned
void AControllableMovableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AControllableMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Time = UGameplayStatics::GetTimeSeconds(GetWorld());
	if (Time > LastTimeMoved + 0.05)
	{
		IsMoving = false;
	} else
	{
		IsMoving = true;
	}

	if (Time < LastTimeAttacked + MeleeAttackDuration)
	{
		IsAttacking = true;
	} else {
		IsAttacking = false;
	}

	if (Time < LastTimeSeed + PlantingDuration)
	{
		if (!IsPlanting)
		{
			Speed = 0;
		}
		IsPlanting = true;
	} else
	{
		if (IsPlanting)
		{
			Speed = BaseSpeed;
		}
		IsPlanting = false;
	}
}

bool AControllableMovableActor::GetCanSeed()
{
	return CanSeed;
}

void AControllableMovableActor::DBG_WinEveryoneInRoom()
{
	WorldMap->TickPlants();
}


int AControllableMovableActor::MoveTo(FVector3f Dir, float DeltaTime, bool WithRotation) {
	LastTimeMoved = UGameplayStatics::GetTimeSeconds(GetWorld());
	
	// location
	if (Speed != 0)
	{
		const FVector Loc = GetActorLocation();
		const FVector3f Loc3F = FVector3f(Loc.X, Loc.Y, Loc.Z);
		const FVector3f DesiredLoc3F = Loc3F + Dir * Speed * DeltaTime;
		FVector3f ActualLoc3F = Loc3F;
		
		if (WorldMap->IsCellOfType((Loc3F.X + 50) / 100, (DesiredLoc3F.Y + 50) / 100, CELLTYPE_OBSTACLE))
		{
			Dir.Y = 0;
		}

		if (WorldMap->IsCellOfType((DesiredLoc3F.X + 50) / 100, (Loc3F.Y + 50) / 100, CELLTYPE_OBSTACLE))
		{
			Dir.X = 0;
		}

		ActualLoc3F += Dir * Speed * DeltaTime;
		SetActorLocation(FVector(ActualLoc3F));

		if (WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100, (Loc3F.Y + 50) / 100, CELLTYPE_SOIL_NO_PLANT))
		{
			CanSeed = true;
		} else
		{
			CanSeed = false;
		}

		if (CanSeed == false)
		{
			CanSeed = WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100 + 1, (Loc3F.Y + 50) / 100, CELLTYPE_SOIL_NO_PLANT)
				|| WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100 - 1, (Loc3F.Y + 50) / 100, CELLTYPE_SOIL_NO_PLANT)
				|| WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100, (Loc3F.Y + 50) / 100 + 1, CELLTYPE_SOIL_NO_PLANT)
				|| WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100, (Loc3F.Y + 50) / 100 - 1, CELLTYPE_SOIL_NO_PLANT);
		}
	}
	
	//rotation
	if (WithRotation) {
		FacingDir.X = FMath::FInterpTo(FacingDir.X, Dir.X, DeltaTime, 10);
		FacingDir.Y = FMath::FInterpTo(FacingDir.Y, Dir.Y, DeltaTime, 10);
		FacingDir.Z = FMath::FInterpTo(FacingDir.Z, Dir.Z, DeltaTime, 10);
		SetActorRotation(FRotator(FacingDir.ToOrientationRotator()));
	}
	
	return WORLD_SUCCESS;
}

const UActorComponent_Inventar* AControllableMovableActor::GetInventarComponent()
{
	return (UActorComponent_Inventar*) GetComponentByClass(UActorComponent_Inventar::StaticClass());
}

void AControllableMovableActor::SelectInventarObject(int ObjIdx)
{	
	UActorComponent_Inventar * Inventar =
		(UActorComponent_Inventar*) GetComponentByClass(UActorComponent_Inventar::StaticClass());

	if (Inventar == nullptr)
	{
		UE_LOG(Player, Warning, TEXT("User selected inventar object, but no inventar associated!"));
		return;
	}

	ObjIdx--;
	ObjIdx %= 10;

	UE_LOG(Player, Error, TEXT("Try to select ObjIdx=%d"),ObjIdx);

	UInventarObject * PreviouslySelectedObject = Inventar->GetSelectedObject();
	
	if (Inventar->SelectAccessBarObject(ObjIdx) != WORLD_SUCCESS)
	{
		UE_LOG(Player, Error, TEXT("Failed to modify selected object!"));
		return;
	}
	
	UInventarObject * SelectedObject = Inventar->GetSelectedObject();

	if (PreviouslySelectedObject != nullptr && PreviouslySelectedObject->HoldableComponent != nullptr)
	{
		USkeletalMeshComponent * Skelet = FindComponentByClass<USkeletalMeshComponent>();
		PreviouslySelectedObject->HoldableComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
		PreviouslySelectedObject->HoldableComponent->SetVisibility(false);
	}
	
	if (SelectedObject != nullptr && SelectedObject->HoldableComponent != nullptr)
	{
		USkeletalMeshComponent * Skelet = FindComponentByClass<USkeletalMeshComponent>();
		
		SelectedObject->HoldableComponent->AttachToComponent(Skelet,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false),
			FName(TEXT("WeaponSocket")));

		SelectedObject->HoldableComponent->SetVisibility(true);
	}
}

void AControllableMovableActor::Action1()
{
	UActorComponent_Inventar * Inventar =
		(UActorComponent_Inventar*) GetComponentByClass(UActorComponent_Inventar::StaticClass());

	if (Inventar == nullptr)
	{
		UE_LOG(Player, Warning, TEXT("User attacks1, but no inventar associated!"));
		return;
	}

	UInventarObject * SelectedObject = Inventar->GetSelectedObject();

	if (SelectedObject == nullptr)
	{
		UE_LOG(Player, Warning, TEXT("User attacks1, but no selected object!"));
		return;
	}
	
	if (SelectedObject->IsMeleeWeapon)
	{
		AttackMelee();
	}
}

void AControllableMovableActor::Action2()
{
}

void AControllableMovableActor::Use()
{
	UActorComponent_Inventar * Inventar =
		(UActorComponent_Inventar*) GetComponentByClass(UActorComponent_Inventar::StaticClass());

	if (Inventar == nullptr)
	{
		UE_LOG(Player, Warning, TEXT("Cannot perform action Use: no inventar associated!"));
		return;
	}

	UInventarObject * SelectedObject = Inventar->GetSelectedObject();

	if (SelectedObject == nullptr)
	{
		UE_LOG(Player, Warning, TEXT("Cannot perform action Use: no selected object!"));
		return;
	}

	if (SelectedObject->Plant != nullptr)
	{
		Plant(SelectedObject->Plant);
		return;
	}

	if (SelectedObject->ObjectName.Equals(FString("Bulb")))
	{
		StartCrafting();
		return;
	}

	UE_LOG(Player, Warning, TEXT("Cannot perform action Use: object is unusable!"));
	return;
}

void AControllableMovableActor::AttackMelee()
{
	float Time = UGameplayStatics::GetTimeSeconds(GetWorld());
	if (LastTimeAttacked + MeleeAttackDuration + 0.3 > Time)
	{
		return;
	}

	LastTimeAttacked = Time;
}

void AControllableMovableActor::StartCrafting()
{
	IsCrafting = true;
	Speed = BaseSpeed / 2;
}

void AControllableMovableActor::Plant(APlant * SelectedPlant)
{
	if (WorldMap == nullptr)
	{
		UE_LOG(Player, Error, TEXT("Cannot perform action Plant: no associated WorldMap"));
		return;
	}
	
	if (SelectedPlant == nullptr)
	{
		UE_LOG(Player, Error, TEXT("Cannot perform action Plant: no plant selected"));
		return;
	}

	if (CanSeed)
	{
		if (WorldMap->AddNewPlant(SelectedPlant, GetActorLocation()) == WORLD_SUCCESS)
		{
			CanSeed = false;
			LastTimeSeed = UGameplayStatics::GetTimeSeconds(GetWorld());
		}
	}
}

bool AControllableMovableActor::GetIsMoving()
{
	return IsMoving;
}

bool AControllableMovableActor::GetIsSeeding()
{
	return IsPlanting;
}

bool AControllableMovableActor::GetIsCrafting()
{
	return IsCrafting;
}

float AControllableMovableActor::GetSpeed()
{
	return Speed;
}

void AControllableMovableActor::SetSpeed(float NewSpeed)
{
	Speed = NewSpeed;
}

bool AControllableMovableActor::GetIsMeleeAttacking()
{
	return IsAttacking;
}
