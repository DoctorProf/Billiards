#pragma once
#include "physics.h"

void generateGrid(int width, int height, Cell** grid);
void generateBalls(Ball* balls, Vector2 win_size, int radius, int* balls_count);
void checkEndStep(Ball* balls, int balls_count, bool* step);
