// Fill out your copyright notice in the Description page of Project Settings.


#include "World/WorldRoom.h"

#include "World/WorldGlobals.h"

AWorldRoom::AWorldRoom()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AWorldRoom::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AWorldRoom::OnOverlapEnd);

	DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale()*100, FColor::Cyan, true, -1, 0, 5);
}

void AWorldRoom::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(WorldRoomGeneration, Log, TEXT("Player Entered the room"));
}

void AWorldRoom::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(WorldRoomGeneration, Log, TEXT("Player Exited the room"));
}
