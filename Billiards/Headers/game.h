#pragma once
#include "physics.h"

void generateGrid(int width, int height, Cell** grid);
int generateBalls(Ball* balls, Vector2 win_size, int radius);
void checkEndStep(Ball* balls, int balls_count, bool* step);
