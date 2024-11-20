#include "../Headers/vectormath.h"

float lengthVector(Vector2 vec)
{
	return sqrtf(powf(vec.x, 2) + powf(vec.y, 2));
}
Vector2 normalizeVector(Vector2 vec)
{
	Vector2 normal;
	float dist = lengthVector(vec);
	normal.x = vec.x / dist;
	normal.y = vec.y / dist;
	return normal;
}
Vector2 scaleVector(Vector2 vec, int value)
{
	vec.x *= value;
	vec.y *= value;
	return vec;
}
Vector2 subtractVectors(Vector2 vec_1, Vector2 vec_2)
{
	Vector2 result = { 0, 0 };
	result.x = vec_1.x - vec_2.x;
	result.y = vec_1.y - vec_2.y;
	return result;
}
Vector2 addVectors(Vector2 vec_1, Vector2 vec_2)
{
	Vector2 result = { 0, 0 };
	result.x = vec_1.x + vec_2.x;
	result.y = vec_1.y + vec_2.y;
	return result;
}
float dotProduct(Vector2 vec_1, Vector2 vec_2)
{
	return vec_1.x * vec_2.x + vec_1.y * vec_2.y;
}
Vector2 reflectVector(Vector2 velocity, Vector2 normal)
{
	normal = normalizeVector(normal);
	float dot = dotProduct(velocity, normal);
	Vector2 reflect_velocity = subtractVectors(velocity, scaleVector(normal, 2.f * dot));
	return reflect_velocity;
}
float diagonal(Vector2 size)
{
	return sqrtf(powf(size.x, 2) + powf(size.y, 2));
}