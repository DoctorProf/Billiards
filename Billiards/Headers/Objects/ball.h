#pragma once
#include <raylib.h>
#include "type.h"

typedef struct
{
	char number[3];
	Vector2 position;
	Vector2 velocity;
	Type type;
} Ball;
