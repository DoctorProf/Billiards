#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Headers/Ball.h"
#include "Headers/Cell.h"
#include <time.h>

double distance(Vector2 pos_ball_1, Vector2 pos_ball_2)
{
	double x = pos_ball_2.x - pos_ball_1.x;
	double y = pos_ball_2.y - pos_ball_1.y;
	return sqrt(pow(x, 2) + pow(y, 2));
}
int generateBalls(Ball* balls, int screen_width, int screen_height, int radius)
{
	int balls_count = 0;
	float start_x = round(screen_width / 1.5);
	float start_y = screen_height / 2;
	int j = 0;
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
void generateGrid(int width, int height, Cell*** grid)
{
	for (int i = 0; i < height; ++i)
	{
		grid[i] = (Cell*)malloc(width * sizeof(Cell));
		for (int j = 0; j < width; ++j)
		{
			grid[i][j]->balls_index = NULL;
		}
	}
}

void checkCells(Cell*** grid, int width, int height, Ball* balls, int balls_count, int radius)
{
	for (int i = 0; i < balls_count; ++i)
	{
		int x = balls[i].position.x / (2 * radius);
		int y = balls[i].position.y / (2 * radius);
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			(*grid)[y][x].balls_index = (int*)malloc(sizeof(int));
			(*grid)[y][x].balls_index = i;
		}
	}
}
void collisionBalls(Ball* balls, int ball_count)
{
}
int main()
{
	float damping = 0.1;
	int screen_width = 800;
	int screen_height = 400;
	int radius = 10;

	int holes_positions[6][2] =
	{
		{radius, radius},
		{screen_width / 2 + radius, radius},
		{screen_width - radius, radius},
		{radius, screen_height - radius},
		{screen_width / 2, screen_height - radius},
		{screen_width - radius, screen_height - radius}
	};

	int size = 16;
	Ball* balls = malloc(size * sizeof(Ball));

	int width_grid = screen_width / (2 * radius);
	int height_grid = screen_height / (2 * radius);

	Cell** grid = (Cell**)malloc(height_grid * sizeof(Cell*));
	if (grid == NULL)
	{
		fprintf(stderr, "Ошибка выделения памяти\n");
		free(balls);
		return 1;
	}
	int balls_count = generateBalls(balls, screen_width, screen_height, radius);

	balls[balls_count].position = (Vector2){ (float)round(screen_width / 2.5) , screen_height / 2 };
	balls[balls_count].velocity = (Vector2){ 0 ,0 };
	balls[balls_count].type = Main;
	Ball* main_ball = &balls[balls_count];
	++balls_count;
	generateGrid(width_grid, height_grid, &grid);
	checkCells(&grid, width_grid, height_grid, balls, balls_count, radius);

	InitWindow(screen_width, screen_height, "Billiards");
	SetTargetFPS(120);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(DARKGREEN);

		for (int i = 0; i < 6; ++i)
		{
			DrawCircle(holes_positions[i][0], holes_positions[i][1], radius, BLACK);
		}
		for (int i = 0; i < balls_count; ++i)
		{
			Ball ball = balls[i];
			DrawCircle(ball.position.x, ball.position.y, radius, ball.type != Main ? WHITE : RED);
			DrawCircleLines(ball.position.x, ball.position.y, radius, BLACK);
		}
		EndDrawing();
	}
	CloseWindow();
	free(balls);
	if (*grid != NULL)
	{
		for (int i = 0; i < height_grid; ++i)
		{
			for (int j = 0; j < width_grid; ++j)
			{
				free(grid[i][j].balls_index);
			}
			free(grid[i]);
		}
		free(*grid);
	}
	return 0;
}