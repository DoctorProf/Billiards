#pragma once
#include "ball.h"
#include "cell.h"

typedef struct
{
	Ball* balls;
	Cell** grid;
	Ball* main_ball;
	Vector2 main_ball_pos;
	Vector2 win_size;
	Vector2 grid_size;
	Vector2* holes_positions;
	int* balls_count;
	bool* grid_show;
	bool* shot;
	bool* step;
	int radius;
	int max_power;
	float damping;
	float* delta;
} GameState;
