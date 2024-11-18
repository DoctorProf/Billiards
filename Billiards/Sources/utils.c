#include "../Headers/utils.h"

float distance(Vector2 pos_1, Vector2 pos_2)
{
	float x = pos_2.x - pos_1.x;
	float y = pos_2.y - pos_1.y;
	return sqrtf(powf(x, 2) + powf(y, 2));
}
Vector2 direction(Vector2 start_pos, Vector2 end_pos)
{
	Vector2 direction;
	float dist = distance(start_pos, end_pos);
	direction.x = (start_pos.x - end_pos.x) / dist;
	direction.y = (start_pos.y - end_pos.y) / dist;
	return direction;
}