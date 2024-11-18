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
int generateBalls(Ball* balls, int screen_width, int screen_height, int radius)
{
	int balls_count = 0;
	float start_x = round(screen_width / 1.5);
	float start_y = screen_height / 2;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j <= i; ++j)
		{
			balls[balls_count].position = (Vector2){ start_x , start_y + 2 * (radius + 1) * j };
			balls[balls_count].velocity = (Vector2){ 0 ,0 };
			balls[balls_count].type = Base;
			++balls_count;
		}
		start_y -= radius;
		start_x += 2 * radius;
	}
	return balls_count;
}
void checkEndRound(Ball* balls, int balls_count, bool* round)
{
	for (int i = 0; i < balls_count; ++i)
	{
		if (lengthVector(balls[i].velocity) != 0)
		{
			*round = true;
			return;
		}
	}
	*round = false;
}