#pragma once
#include <stdlib.h>
#include "Objects/Cell.h"
#include "Objects/Ball.h"
#include "vectormath.h"
#include "utils.h"

void checkCells(Cell** grid, int width_grid, int height_grid, Ball* balls, int balls_count, int radius);
void moveBalls(Ball* balls, int balls_count, float damping, float delta);
void moveBall(Ball* ball, Vector2 step);
void collisionScreen(Ball* balls, int balls_count, int radius, int screen_width, int screen_height, float delta);
bool collide(Vector2 pos_1, Vector2 pos_2, int radius);
void solveCollision(Ball* current_ball, Ball* other_ball, int radius);
void checkCellsCollision(Ball* balls, Cell* current_cell, Cell* other_cell, int radius);
void findCollisionGrid(Ball* balls, Cell** grid, int width_grid, int height_grid, int radius);
void collisionBalls(Ball* balls, int balls_count, int radius);

void collisionHoles(Ball* balls, int* balls_count, int radius, Vector2 holes_positions[6], Vector2 main_ball_pos);
