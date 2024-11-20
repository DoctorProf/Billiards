#pragma once
#include <math.h>
#include <raylib.h>

float lengthVector(Vector2 vec);
Vector2 normalizeVector(Vector2 vec);
Vector2 scaleVector(Vector2 vec, int value);
Vector2 subtractVectors(Vector2 vec_1, Vector2 vec_2);
Vector2 addVectors(Vector2 vec_1, Vector2 vec_2);
float dotProduct(Vector2 vec_1, Vector2 vec_2);
Vector2 reflectVector(Vector2 velocity, Vector2 normal);
float diagonal(Vector2 size);