// Fill out your copyright notice in the Description page of Project Settings.


#include "World/WorldRoomGenerator.h"

#include "WorldMapCells.h"
#include "World/WorldGlobals.h"
#include "World/WorldRandom.h"

// Sets default values
AWorldRoomGenerator::AWorldRoomGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWorldRoomGenerator::BeginPlay()
{
	Super::BeginPlay();
}

int AWorldRoomGenerator::GenerateRooms(WorldMapCells* MapCells)
{
	if (MapCells == NULL)
	{
		UE_LOG(WorldRoomGeneration, Error, TEXT("Could not generate rooms - cells is null"));
		return WORLD_FAILURE;
	}

	if (MapCells->GetSize() == 0)
	{
		UE_LOG(WorldRoomGeneration, Warning, TEXT("Room generation not required - map of size 0"));
		return WORLD_SUCCESS;
	}

	this->Cells = MapCells;

	if (RoomSizes[0] == 0)
	{
		UE_LOG(WorldRoomGeneration, Warning, TEXT("Room generation not required - the biggest room size is 0"));
		return WORLD_FAILURE;
	}

	unsigned int i = 1;
	while (RoomSizes[i] != 0 && i < MAX_ROOM_TYPES)
	{
		if (RoomSizes[i] > RoomSizes[i-1])
		{
			UE_LOG(WorldRoomGeneration, Error, TEXT("Could not generate rooms - room %u is bigger than room %u (%u > %u)"), i, i-1, RoomSizes[i], RoomSizes[i-1]);
			return WORLD_FAILURE;
		}
		i++;
	}

	Rooms.Init(NO_ROOM,MapCells->GetSize() * MapCells->GetSize());

	i = 0;
	unsigned int PreviousRoomSize = 0;
	unsigned int MapSize = Cells->GetSize();
	
	while (i < MAX_ROOM_TYPES)
	{
		unsigned int RoomSize = RoomSizes[i];

		if (RoomSize == 0)
			break;

		for (unsigned int X_Iter = RoomSize + 1 ; X_Iter < (MapSize - RoomSize - 1) ; X_Iter++)
		{
			for (unsigned int Y_Iter = RoomSize + 1 ; Y_Iter < (MapSize - RoomSize - 1) ; Y_Iter++)
			{
				unsigned int pos = Cells->D2ToD1(X_Iter, Y_Iter);
				
				if (Rooms[pos] == NO_ROOM)
				{
					GenerationPointsAvailable[i]++;
					Rooms[pos] = i;
				} else
				{
					Y_Iter += PreviousRoomSize;
				}
			}
		}

		PreviousRoomSize = RoomSize;
		i++;
	}

	for (i = 0 ; i < MAX_ROOM_TYPES - 1 ; i++)
	{
		if (RoomSizes[i+1] == 0)
		{
			UE_LOG(WorldRoomGeneration, Log, TEXT("There are %u possible spawn points for room with size %u"), GenerationPointsAvailable[i], RoomSizes[i]);
			break;
		}
		
		GenerationPointsAvailable[i+1] += GenerationPointsAvailable[i];
		UE_LOG(WorldRoomGeneration, Log, TEXT("There are %u possible spawn points for room with size %u"), GenerationPointsAvailable[i], RoomSizes[i]);
	}

	for (i = 0 ; i < MAX_ROOM_TYPES ; i++)
	{
		if (RoomSizes[i] == 0)
		{
			break;
		}

		if (RoomAmount[i] == 0)
		{
			continue;;
		}

		unsigned int ThisRoomAmount = RoomAmount[i];
		unsigned int ThisRoomSize = RoomSizes[i];
		
		for (unsigned int Room_Iter = 0 ; Room_Iter < ThisRoomAmount ; Room_Iter++)
		{
			if (GenerationPointsAvailable[i] == 0)
			{
				UE_LOG(WorldRoomGeneration, Warning, TEXT("Generating room nr.%u of size %u, but no space available! Skipping..."),Room_Iter,RoomSizes[i]);
				continue;
			}
			
			UE_LOG(WorldRoomGeneration, Log, TEXT("Generating room nr.%u of size %u"),Room_Iter,RoomSizes[i]);

			unsigned int SpawnNumber = 0;
			Random(SpawnNumber);
			SpawnNumber %= GenerationPointsAvailable[i];

			unsigned int SpawnPosition = 0;
			while (SpawnNumber > 0 && SpawnPosition < Cells->GetSize()*Cells->GetSize())
			{
				if (Rooms[SpawnPosition] <= ThisRoomSize && Rooms[SpawnPosition] != NO_ROOM)
				{
					SpawnNumber--;
				}
				SpawnPosition++;
			}

			unsigned int Room_X, Room_Y;
			Cells->D1ToD2(SpawnPosition, Room_X, Room_Y);
			UE_LOG(WorldRoomGeneration, Log, TEXT("Room position is %u (X=%u, Y=%u)"),SpawnPosition, Room_X, Room_Y);

			unsigned int BiggestRoom = RoomSizes[0];
			unsigned int CellsDisabledByThisRoom = 0;

			unsigned int X_Iter_Min = Room_X - ThisRoomSize - BiggestRoom;
			if (X_Iter_Min >= Cells->GetSize()) X_Iter_Min = 0;
			unsigned int X_Iter_Max = Room_X + ThisRoomSize + BiggestRoom;
			if (X_Iter_Max >= Cells->GetSize()) X_Iter_Max = Cells->GetSize()-1;
			
			for (unsigned int X_Iter = X_Iter_Min; X_Iter <= X_Iter_Max ; X_Iter++)
			{
				unsigned int Y_Iter_Min = Room_Y - ThisRoomSize - BiggestRoom;
				if (Y_Iter_Min >= Cells->GetSize()) Y_Iter_Min = 0;
				unsigned int Y_Iter_Max = Room_Y + ThisRoomSize + BiggestRoom;
				if (Y_Iter_Max >= Cells->GetSize()) Y_Iter_Max = Cells->GetSize() - 1;
				
				for (unsigned int Y_Iter = Y_Iter_Min; Y_Iter <= Y_Iter_Max ; Y_Iter++)
				{
					if (Rooms[Cells->D2ToD1(X_Iter,Y_Iter)] != NO_ROOM)
						CellsDisabledByThisRoom++;
					Rooms[Cells->D2ToD1(X_Iter,Y_Iter)] = NO_ROOM;

					if (X_Iter > Room_X - ThisRoomSize && X_Iter < Room_X + ThisRoomSize
						&& Y_Iter > Room_Y - ThisRoomSize && Y_Iter < Room_Y + ThisRoomSize)
					{
						Cells->SetCellType(X_Iter, Y_Iter, CELLTYPE_EMPTY);
					}
				}
			}

			Cells->AddRoom(SpawnPosition, ThisRoomSize * 2 + 2);
			
			UE_LOG(WorldRoomGeneration, Log, TEXT("This Room Disables %u cells"), CellsDisabledByThisRoom);
			
			for (int GenerationPointsAvailable_Iter = 0 ;
				GenerationPointsAvailable_Iter < MAX_ROOM_TYPES ;
				GenerationPointsAvailable_Iter++)
			{
				GenerationPointsAvailable[GenerationPointsAvailable_Iter] =
					GenerationPointsAvailable[GenerationPointsAvailable_Iter] <= CellsDisabledByThisRoom ?
						0 : GenerationPointsAvailable[GenerationPointsAvailable_Iter] - CellsDisabledByThisRoom;
			
				// if (RoomSizes[GenerationPointsAvailable_Iter] != 0)
				// 	UE_LOG(WorldRoomGeneration, Log, TEXT("After this room there are %u cells available for rooms of size %u"),
				// 	GenerationPointsAvailable[GenerationPointsAvailable_Iter],
				// 	RoomSizes[GenerationPointsAvailable_Iter]);
			}
		}
	}

	return WORLD_SUCCESS;
}


// Called every frame
void AWorldRoomGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

