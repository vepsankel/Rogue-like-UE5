// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(WorldGeneration, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(WorldMazeGeneration, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(WorldRoomGeneration, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(Player, Log, All);

#define DirToY(dir) (((dir) == DIRECTION_UP) ? 1 : (((dir) == DIRECTION_DOWN) ? -1 : 0))
#define DirToX(dir) (((dir) == DIRECTION_RIGHT) ? 1 : (((dir) == DIRECTION_LEFT) ? -1 : 0))

//#define DirToXY(dir, x, y) ((x) = (DirToX(dir)); (y) = (DirToY(dir)))

#define WORLD_SUCCESS 0
#define WORLD_FAILURE 1

enum Direction {
	DIRECTION_NONE = 0,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

#ifndef MAP_SIZE
#define MAP_SIZE 2000
#endif // !MAP_SIZE

#ifndef DIRECTIONS
//#include "Direction.h"
#endif

//#include "Random.h"
