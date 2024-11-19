#pragma once
#include "physics.h"

void generateGrid(int width, int height, Cell** grid);
int generateBalls(Ball* balls, int screen_width, int screen_height, int radius);
void checkEndRound(Ball* balls, int balls_count, bool* round);
