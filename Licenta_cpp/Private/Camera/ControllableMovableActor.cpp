// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/ControllableMovableActor.h"

#include <string>

#include "AnimationCompression.h"
#include "Components/SplineComponent.h"
#include "Potion/PotionProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "ToolDataVisualizer.h"

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

	InventarComponent = GetInventarComponent();
	PotionRecipe = NewObject<UPotionRecipe>(UPotionRecipe::StaticClass());

	IsSpawned = false;
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

	if (Time < LastTimeThrew + ThrowingDuration)
	{
		IsThrowing = true;
	} else
	{
		IsThrowing = false;
	}

	if (IsCrafting)
	{
		DrawAimingSpline();
	}

	if (!IsSpawned)
	{
		FVector PlayerStartPos;
		if (WorldMap->GetPlayerSpawnRoomPos(PlayerStartPos) == WORLD_SUCCESS)
		{
			SetActorLocation(PlayerStartPos);
			IsSpawned = true;
		}
	}
}

bool AControllableMovableActor::GetCanSeed()
{
	return CanSeed;
}

bool AControllableMovableActor::GetCanReap()
{
	return CanReap;
}

void AControllableMovableActor::DBG_WinEveryoneInRoom()
{
	WorldMap->TickPlants();
}


int AControllableMovableActor::MoveTo(FVector3f Dir, float DeltaTime, bool WithRotation) {
	// no effect if is throwing or seeding
	if (IsThrowing || IsPlanting)
	{
		return WORLD_SUCCESS;
	}
	
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

		// check if character can plant
		UpdateCanSeed();

		// check if character can reap
		UpdateCanReap();
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

UActorComponent_Inventar* AControllableMovableActor::GetInventarComponent()
{
	return (UActorComponent_Inventar*) GetComponentByClass(UActorComponent_Inventar::StaticClass());
}

void AControllableMovableActor::SelectInventarObject(int ObjIdx)
{
	if (IsThrowing || IsAttacking || IsPlanting)
	{
		return;
	}
	
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

	// separate logic when selecting ingredients
	if (IsCrafting)
	{
		if (PotionRecipe == nullptr)
		{
			UE_LOG(Player, Warning, TEXT("Cannot perform action Select: Player is crafting, but no writable recipe"));
			return;
		}
		
		UInventarObject * IngredientObj = Inventar->GetObjectByIdx(ObjIdx);

		if (IngredientObj == nullptr)
		{
			UE_LOG(Player, Warning, TEXT("Cannot perform action Select: Player is crafting, but no object found"));
			return;
		}
		
		if (!IngredientObj->GetIsIngridient())
		{
			UE_LOG(Player, Warning, TEXT("Cannot perform action Select: Player is crafting, but selected object is not an ingredient"));
			return;
		}

		unsigned int * PIngredientQuantity = PotionRecipe->IngredientToQuantity.Find(IngredientObj->GetObjectName());

		if (PIngredientQuantity == nullptr)
		{
			PotionRecipe->IngredientToQuantity.Add(IngredientObj->GetObjectName(), 1);
		} else
		{
			(*PIngredientQuantity)++;
		}

		FString RecipeContent = PotionRecipe->GetRecipeContent();

		if (Inventar->ModifyObjectDescription(FString("Bulb"), RecipeContent) == WORLD_FAILURE)
		{
			UE_LOG(Player, Warning, TEXT("Error while performing action Select: Player is crafting, but cannot update bulb description"));
			return;
		} else
		{
			Inventar->Content["Bulb"]->IsThrowable = true;
		}
		
		return;
	}
	
	UInventarObject * PreviouslySelectedObject = Inventar->GetSelectedObject();
	
	if (Inventar->SelectAccessBarObject(ObjIdx) != WORLD_SUCCESS)
	{
		UE_LOG(Player, Error, TEXT("Failed to modify selected object!"));
		return;
	}
	
	SelectedInventarObject = Inventar->GetSelectedObject();

	if (PreviouslySelectedObject != nullptr && PreviouslySelectedObject->HoldableComponent != nullptr)
	{
		USkeletalMeshComponent * Skelet = FindComponentByClass<USkeletalMeshComponent>();
		PreviouslySelectedObject->HoldableComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
		PreviouslySelectedObject->HoldableComponent->SetVisibility(false);
	}
	
	if (SelectedInventarObject != nullptr && SelectedInventarObject->HoldableComponent != nullptr)
	{
		USkeletalMeshComponent * Skelet = FindComponentByClass<USkeletalMeshComponent>();
		
		SelectedInventarObject->HoldableComponent->AttachToComponent(Skelet,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false),
			FName(TEXT("WeaponSocket")));

		SelectedInventarObject->HoldableComponent->SetVisibility(true);
	}

	if (SelectedInventarObject != PreviouslySelectedObject)
	{
		UpdateCanSeed();
		UpdateCanReap();
	}
}

void AControllableMovableActor::Action1()
{
	if (IsThrowing || IsAttacking || IsPlanting)
	{
		return;
	}
	
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

	if (SelectedObject->IsThrowable)
	{
		Throw();
	}
}

// Aim
bool AControllableMovableActor::Action2(bool IsActive, const FRotator & Rotator)
{
	IsAiming = IsActive;
	AimingDir = Rotator;
	
	USkeletalMeshComponent * SkelMesh = (USkeletalMeshComponent*)GetComponentByClass(USkeletalMeshComponent::StaticClass());
	if (SkelMesh == nullptr)
	{
		return true;
	}

	if (!IsActive)
	{
		SkelMesh->SetRelativeLocation(FVector(0,0,0));
		return true;
	}

	if (!IsCrafting)
	{
		SkelMesh->SetRelativeLocation(FVector(0,0,0));
		return false;
	}
	
	if (IsActive)
	{
		SkelMesh->SetRelativeLocation(FVector(-1000,0,0));
	}

	FacingDir = Rotator.Vector();
	SetActorRotation(Rotator);

	return true;
}

void AControllableMovableActor::Use()
{
	if (SelectedInventarObject == nullptr)
	{
		UE_LOG(Player, Warning, TEXT("Cannot perform action Use: no selected object!"));
		return;
	}

	if (CanSeed)
	{
		Plant();
		return;
	}

	if (CanReap)
	{
		Reap();
		return;
	}

	if (SelectedInventarObject->ObjectName.Equals(FString("Bulb")))
	{
		if (IsCrafting)
		{
			EndCrafting();
		} else
		{
			StartCrafting();
		}
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

void AControllableMovableActor::Throw()
{
	if (SelectedInventarObject == nullptr)
	{
		UE_LOG(Player, Error, TEXT("Cannot perform action throw: no selected object"));
		return;
	}

	if (!SelectedInventarObject->IsThrowable)
	{
		UE_LOG(Player, Error, TEXT("Cannot perform action throw: object is not throwable"));
		return;
	}

	float Time = UGameplayStatics::GetTimeSeconds(GetWorld());

	LastTimeThrew = Time;
	ThrowingDelayReached = false;
	
	FActorSpawnParameters params;
	GetWorld()->SpawnActor<APotionProjectile>(Projectile, GetActorLocation() + FVector(0,0,100), AimingDir, params);
	EndCrafting();
}

void AControllableMovableActor::StartCrafting()
{
	BulbPreviousDescription = SelectedInventarObject->GetItemDescription();
	InventarComponent->ModifyObjectDescription("Bulb", "Select ingredients to add to potion!");
	IsCrafting = true;
	Speed = BaseSpeed * 0.7;
}

void AControllableMovableActor::EndCrafting()
{
	InventarComponent->ModifyObjectDescription("Bulb", BulbPreviousDescription);
	IsCrafting = false;
	Speed = BaseSpeed;
	PotionRecipe->IngredientToQuantity.Reset();
}

void AControllableMovableActor::Plant()
{
	APlant * SelectedPlant = SelectedInventarObject->Plant;
	
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
	
	if (InventarComponent == nullptr)
	{
		UE_LOG(Player, Error, TEXT("Cannot perform action Plant: no inventar component attached"));
		return;	
	}

	if (CanSeed)
	{
		if (WorldMap->AddNewPlant(SelectedPlant, GetActorLocation()) == WORLD_SUCCESS)
		{
			CanSeed = false;
			LastTimeSeed = UGameplayStatics::GetTimeSeconds(GetWorld());

			if (SelectedInventarObject->GetHasQuantity())
			{
				InventarComponent->ModifyObjectQuantity(
					SelectedInventarObject->GetObjectName(),
					SelectedInventarObject->GetQuantity() - 1);

				if (SelectedInventarObject->GetQuantity() == 0)
				{
					SelectedInventarObject = nullptr;
				}
			}
		}
	}
}

void AControllableMovableActor::Reap()
{
	if (WorldMap == nullptr)
	{
		UE_LOG(Player, Error, TEXT("Cannot perform action Plant: no associated WorldMap"));
		return;
	}

	if (CanReap)
	{
		const APlant * Plant;
		
		if (WorldMap->ReapPlant(GetActorLocation(), &Plant) == WORLD_SUCCESS)
		{
			// Do something with plant
			UE_LOG(Player, Log, TEXT("Plant %s reaped succesfully!"), *Plant->PlantName);
		} else
		{
			UE_LOG(Player, Error, TEXT("Could not reap plant!"));
			return;
		}

		const FString Name = Plant->PlantName;

		if (InventarComponent == nullptr)
		{
			UE_LOG(Player, Error, TEXT("No inventar component!"));
			return;
		}
		
		if (Name.Equals("HoneyWeed"))
		{
			InventarComponent->ModifyObjectQuantity(FString("HoneyWeed"), 10, true);
		}
	}
}

void AControllableMovableActor::DrawAimingSpline()
{
	return; 
	USplineComponent * SplineComponent = (USplineComponent *)GetComponentByClass(USplineComponent::StaticClass());

	if (SplineComponent == nullptr)
	{
		return;
	}

	// int32 SplinePoints = SplineComponent->GetNumberOfSplinePoints();
	// for (; SplinePoints < 10 ; SplinePoints++)
	// {
	// 	FSplinePoint SplinePoint = FSplinePoint();
	// 	SplineComponent->AddPoint(SplinePoint);
	// }

	FPredictProjectilePathParams Params;

	Params.LaunchVelocity = AimingDir.Vector() * 500;
	Params.MaxSimTime = 10;
	Params.bTraceWithChannel = true;
	Params.bTraceWithCollision = false;
	Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	Params.TraceChannel = ECC_Visibility;

	FPredictProjectilePathResult Result;
	

	UGameplayStatics::PredictProjectilePath(
		GetWorld(),
		Params,
		Result
		);

	TArray<FVector> SplinePointsPos;
	SplinePointsPos.SetNum(10);

	const float VerticalStartVelocity = AimingDir.Vector().Z * 500;
	const float HorizontalStartVelocity = AimingDir.Vector().X * 500;
	UE_LOG(Player, Log, TEXT("AimingDir %s, VerticalStartVelocity=%f"),*AimingDir.Vector().ToString(), VerticalStartVelocity);
	float g = 9.8;
	const float VerticalStartPosition = 0;
	const float HorizontalStartPosition = 0;
	
	
	float time = 0;
	float dt = 0.1;

	for (int n = 0 ; n < 10 ; n++)
	{
		time += dt;

		float height = ((-2*VerticalStartVelocity*g - VerticalStartPosition*g*g) / (4*VerticalStartVelocity*VerticalStartVelocity)) * time * time +
			VerticalStartVelocity * time +
				VerticalStartPosition;

		float distance = HorizontalStartPosition + HorizontalStartVelocity * time;

		// UE_LOG(Player, Log, TEXT("Generated point %d (%f %f %f)"),n,distance,0,height);
		
		SplinePointsPos[n] = FVector(distance, 0, height);
		UE_LOG(Player, Log, TEXT("SplinePoint %s"),*SplinePointsPos[n].ToString());
	}
	
	//SplineComponent->SetSplinePoints(SplinePointsPos, ESplineCoordinateSpace::World);
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

bool AControllableMovableActor::GetIsThrowing()
{
	return IsThrowing;
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

void AControllableMovableActor::UpdateCanReap()
{
	const FVector ActualLoc3F = GetActorLocation();
	
	if (SelectedInventarObject != nullptr && SelectedInventarObject->GetObjectName().Equals("Sword"))
	{
		if (WorldMap->IsPlantFullyGrown(FIntVector((ActualLoc3F.X + 50) / 100, (ActualLoc3F.Y + 50) / 100, 0)))
		{
			CanReap = true;
		} else
		{
			CanReap = false;
		}
	} else
	{
		CanReap = false;
	}
}

void AControllableMovableActor::UpdateCanSeed()
{
	const FVector ActualLoc3F = GetActorLocation();
	
	if (SelectedInventarObject != nullptr && SelectedInventarObject->Plant != nullptr)
	{
		if (WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100, (ActualLoc3F.Y + 50) / 100, CELLTYPE_SOIL_NO_PLANT))
		{
			CanSeed = true;
		} else
		{
			CanSeed = false;
		}

		if (CanSeed == false)
		{
			CanSeed = WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100 + 1, (ActualLoc3F.Y + 50) / 100, CELLTYPE_SOIL_NO_PLANT)
				|| WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100 - 1, (ActualLoc3F.Y + 50) / 100, CELLTYPE_SOIL_NO_PLANT)
				|| WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100, (ActualLoc3F.Y + 50) / 100 + 1, CELLTYPE_SOIL_NO_PLANT)
				|| WorldMap->IsCellOfType((ActualLoc3F.X + 50) / 100, (ActualLoc3F.Y + 50) / 100 - 1, CELLTYPE_SOIL_NO_PLANT);
		}
	} else
	{
		CanSeed = false;
	}
}

