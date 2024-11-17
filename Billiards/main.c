#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Headers/Ball.h"
#include "Headers/Cell.h"
#include <time.h>
#include <stdbool.h>

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

void checkCells(Cell** grid, int width, int height, Ball* balls, int balls_count, int radius)
{
	for (int i = 0; i < balls_count; ++i)
	{
		int x = balls[i].position.x / (2 * radius);
		int y = balls[i].position.y / (2 * radius);
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			grid[y][x].balls_index = (int*)realloc(grid[y][x].balls_index, sizeof(int));
			grid[y][x].balls_index[grid[y][x].count_balls] = i;
			grid[y][x].count_balls += 1;
		}
	}
}
void collisionBalls(Ball* balls, int ball_count)
{
}
void moveBalls(Ball* balls, int balls_count, float damping)
{
	for (int i = 0; i < balls_count; ++i)
	{
		//balls[i].velocity.x = balls[i].velocity.x * damping;
		//balls[i].velocity.y = balls[i].velocity.y * damping;
		balls[i].position.x += balls[i].velocity.x;
		balls[i].position.y += balls[i].velocity.y;
	}
}
void collisionScreen()
{
}
int main()
{
	srand(time(NULL));
	float damping = 0.01;
	int screen_width = 800;
	int screen_height = 400;
	int radius = 10;

	int holes_positions[6][2] =
	{
		{radius, radius},
		{screen_width / 2 + radius, radius},
		{screen_width - radius, radius},
		{radius, screen_height - radius},
		{screen_width / 2 + radius, screen_height - radius},
		{screen_width - radius, screen_height - radius}
	};

	int size = 16;
	Ball* balls = malloc(size * sizeof(Ball));

	int width_grid = screen_width / (2 * radius);
	int height_grid = screen_height / (2 * radius);

	Cell** grid = (Cell**)malloc(height_grid * sizeof(Cell*));
	if (grid == NULL)
	{
		fprintf(stderr, "������ ��������� ������\n");
		free(balls);
		return 1;
	}
	int balls_count = generateBalls(balls, screen_width, screen_height, radius);

	balls[balls_count].position = (Vector2){ (float)round(screen_width / 3.5) , screen_height / 2 };
	balls[balls_count].velocity = (Vector2){ 0 ,0 };
	balls[balls_count].type = Main;

	Ball* main_ball = &balls[balls_count++];

	generateGrid(width_grid, height_grid, grid);
	checkCells(grid, width_grid, height_grid, balls, balls_count, radius);
	InitWindow(screen_width, screen_height, "Billiards");
	SetTargetFPS(60);

	for (int i = 0; i < height_grid; ++i)
	{
		for (int j = 0; j < width_grid; ++j)
		{
			if (grid[i][j].count_balls <= 0) continue;
			printf("Cell row - %d, column - %d ", i, j);
			printf("Size - %d ", grid[i][j].count_balls);
			for (int k = 0; k < grid[i][j].count_balls; ++k)
			{
				printf("Index %d ", grid[i][j].balls_index[k]);
			}
			printf("\n");
		}
	}
	bool pause = false;
	bool shot = false;
	Vector2 current_mouse_pos = { 0, 0 };
	float max_power = 200;

	while (!WindowShouldClose())
	{
		current_mouse_pos = GetMousePosition();
		if (IsKeyPressed(KEY_SPACE))
		{
			pause = !pause;
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (distance(current_mouse_pos, main_ball->position) <= radius && !shot)
			{
				shot = true;
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && shot)
		{
			shot = false;
			Vector2 dir = direction(main_ball->position, current_mouse_pos);
			main_ball->velocity.x = dir.x;
			main_ball->velocity.y = dir.y;
		}
		if (!pause)
		{
			moveBalls(balls, balls_count, damping);
		}

		BeginDrawing();
		ClearBackground(DARKGREEN);
		for (int i = 0; i < height_grid; ++i)
		{
			for (int j = 0; j < width_grid; ++j)
			{
				DrawRectangle(j * radius * 2, i * radius * 2, radius * 2, radius * 2, DARKGREEN);
				DrawRectangleLines(j * radius * 2, i * radius * 2, radius * 2, radius * 2, BLACK);
			}
		}

		for (int i = 0; i < 6; ++i)
		{
			DrawCircle(holes_positions[i][0], holes_positions[i][1], radius, BLACK);
		}
		for (int i = 0; i < balls_count; ++i)
		{
			Ball ball = balls[i];
			DrawCircle(ball.position.x, ball.position.y, radius, ball.type != Main ? WHITE : RED);
			DrawCircleLines(ball.position.x, ball.position.y, radius, BLACK);
			if (i != 15)
			{
				Font font = GetFontDefault();
				char buffer[3];
				snprintf(buffer, sizeof(buffer), "%d", i + 1);
				char* text = buffer;
				int font_size = 16;
				int spacing = 2;
				Vector2 size_text = MeasureTextEx(font, text, font_size, spacing);
				Vector2 text_pos = { ball.position.x , ball.position.y };
				Vector2 origin = { size_text.x / 2, size_text.y / 2 };
				DrawTextPro(font, text, text_pos, origin, 0, font_size, spacing, BLACK);
			}
		}
		if (shot)
		{
			int width = screen_width / 40;
			int height = screen_height / 5;
			int pos_x = 10;
			int pos_y = screen_height / 2 - height / 2;
			DrawRectangle(pos_x, pos_y, width, height, WHITE);
			float dist = distance(current_mouse_pos, main_ball->position);
			int height_power = (int)(dist * (height / max_power));
			height_power = height_power > height ? height : height_power;
			int pos_y_power = height - height_power;
			DrawRectangle(pos_x, pos_y + pos_y_power, width, height_power, RED);

			Vector2 dir = direction(main_ball->position, current_mouse_pos);
			Vector2 end_pos;
			end_pos.x = main_ball->position.x + 100 * dir.x;
			end_pos.y = main_ball->position.y + 100 * dir.y;
			DrawLineV(main_ball->position, end_pos, WHITE);
			printf("X - %f, Y - %f\n", current_mouse_pos.x, current_mouse_pos.y);
		}
		if (pause)
		{
			Font font = GetFontDefault();
			char* text = "PAUSE";
			int font_size = 50;
			int spacing = 5;
			Vector2 size_text = MeasureTextEx(font, text, font_size, spacing);
			Vector2 text_pos = { screen_width / 2, screen_height / 2 };
			Vector2 origin = { size_text.x / 2, size_text.y / 2 };
			DrawTextPro(font, text, text_pos, origin, 0, font_size, spacing, WHITE);
		}
		EndDrawing();
	}
	CloseWindow();
	free(balls);
	free(grid);
	return 0;
}