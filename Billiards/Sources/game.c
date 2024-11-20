#include "../Headers/game.h"

void generateGrid(int width, int height, Cell** grid)
{
	for (int i = 0; i < height; ++i)
	{
		grid[i] = (Cell*)malloc(width * sizeof(Cell));
		for (int j = 0; j < width; ++j)
		{
			grid[i][j].balls_index = NULL;
			grid[i][j].count_balls = 0;
		}
	}
}
void generateBalls(Ball* balls, Vector2 win_size, int radius, int* balls_count)
{
	float start_x = round(win_size.x / 1.5);
	float start_y = win_size.y / 2;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j <= i; ++j)
		{
			snprintf(balls[*balls_count].number, sizeof(balls[*balls_count].number), "%d", *balls_count + 1);
			balls[*balls_count].position = (Vector2){ start_x, start_y + 2 * (radius + 1) * j };
			balls[*balls_count].velocity = (Vector2){ 0 ,0 };
			balls[*balls_count].type = Base;
			++(*balls_count);
		}
		start_y -= radius;
		start_x += 2 * radius;
	}
}
void checkEndStep(Ball* balls, int balls_count, bool* step)
{
	for (int i = 0; i < balls_count; ++i)
	{
		if (lengthVector(balls[i].velocity) != 0)
		{
			(*step) = true;
			return;
		}
	}
	(*step) = false;
}