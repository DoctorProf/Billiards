#include <stdio.h>
#include "Headers/game.h"
#include "Headers/utils.h"
#include <stdbool.h>
#include <pthread.h>

void* gameLogic(Ball* main_ball, Ball* balls, Cell** grid, Vector2 win_size, Vector2 grid_size,
	Vector2 holes_positions[6], Vector2 main_ball_pos,
	int* balls_count, bool* grid_show, bool* shot, bool* step, int radius, int max_power,
	int diagonal, int damping)
{
	while (!WindowShouldClose)
	{
		main_ball = &balls[*balls_count - 1];
		if (IsKeyPressed(KEY_G))
		{
			grid_show = !grid_show;
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !shot && !step)
		{
			Vector2 mouse_pos = GetMousePosition();
			if (distance(mouse_pos, main_ball->position) <= radius)
			{
				shot = true;
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && shot)
		{
			Vector2 mouse_pos = GetMousePosition();
			shot = false;
			Vector2 dir = direction(main_ball->position, mouse_pos);
			float coef = distance(main_ball->position, mouse_pos) / diagonal;
			float power = max_power * coef;
			power = power > max_power ? max_power : power;
			main_ball->velocity.x = dir.x * power;
			main_ball->velocity.y = dir.y * power;
			step = true;
		}
		moveBalls(balls, *balls_count, damping);
		checkCells(grid, grid_size.x, grid_size.y, balls, *balls_count, radius);
		findCollisionGrid(balls, grid, grid_size.x, grid_size.y, radius);
		collisionScreen(balls, *balls_count, radius, win_size.x, win_size.y);
		collisionHoles(balls, balls_count, radius, holes_positions, main_ball_pos);
		checkEndStep(balls, *balls_count, step);
		Sleep(1);
	}
	return NULL;
}
int main()
{
	Vector2 win_size = { 800, 400 };
	int diagonal = sqrtf(powf(win_size.x, 2) + powf(win_size.y, 2)) / 2;
	int radius = 10;
	int size = 16;
	float damping = 0.99;

	Vector2 grid_size = { win_size.x / (2 * radius), win_size.y / (2 * radius) };

	bool grid_show = false;
	bool shot = false;
	bool step = false;
	float max_power = 30;

	Vector2 holes_positions[6] =
	{
		{radius, radius},
		{win_size.x / 2 + radius, radius},
		{win_size.x - radius, radius},
		{radius, win_size.y - radius},
		{win_size.x / 2 + radius, win_size.y - radius},
		{win_size.x - radius, win_size.y - radius}
	};

	Cell** grid = (Cell**)malloc(grid_size.y * sizeof(Cell*));
	generateGrid(grid_size.x, grid_size.y, grid);

	Ball* balls = malloc(size * sizeof(Ball));
	int balls_count = generateBalls(balls, win_size, radius);

	Vector2  main_ball_pos = { round(win_size.x / 3.5), win_size.y / 2 };
	balls[balls_count].position = main_ball_pos;
	balls[balls_count].velocity = (Vector2){ 0 ,0 };
	balls[balls_count].type = Main;
	snprintf(balls[balls_count].number, sizeof(balls[balls_count].number), "%d", balls_count + 1);

	Ball* main_ball = &balls[balls_count++];

	InitWindow(win_size.x, win_size.y, "Billiards");
	SetTargetFPS(60);
	SetWindowState(FLAG_VSYNC_HINT);

	pthread_t logicThread;
	//pthread_create(&logicThread, NULL, gameLogic, args);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(DARKGREEN);
		if (grid_show)
		{
			for (int i = 0; i < grid_size.y; ++i)
			{
				for (int j = 0; j < grid_size.x; ++j)
				{
					DrawRectangle(j * radius * 2, i * radius * 2, radius * 2, radius * 2, DARKGREEN);
					DrawRectangleLines(j * radius * 2, i * radius * 2, radius * 2, radius * 2, BLACK);
				}
			}
		}
		for (int i = 0; i < 6; ++i)
		{
			DrawCircle(holes_positions[i].x, holes_positions[i].y, radius, BLACK);
		}
		for (int i = 0; i < balls_count; ++i)
		{
			Ball ball = balls[i];
			DrawCircle(ball.position.x, ball.position.y, radius, ball.type != Main ? WHITE : RED);
			DrawCircleLines(ball.position.x, ball.position.y, radius, BLACK);
			//
			if (i != balls_count - 1)
			{
				Font font = GetFontDefault();
				int font_size = 16;
				int spacing = 2;
				char* text = ball.number;
				Vector2 size_text = MeasureTextEx(font, text, font_size, spacing);
				Vector2 text_pos = { ball.position.x , ball.position.y };
				Vector2 origin = { size_text.x / 2, size_text.y / 2 };
				DrawTextPro(font, text, text_pos, origin, 0, font_size, spacing, BLACK);
			}
		}
		if (shot)
		{
			Vector2 mouse_pos = GetMousePosition();
			int width = win_size.x / 40;
			int height = win_size.y / 5;
			int pos_x = 10;
			int pos_y = win_size.y / 2 - height / 2;
			DrawRectangle(pos_x, pos_y, width, height, WHITE);
			float coef = distance(main_ball->position, mouse_pos) / diagonal;
			float power = max_power * coef;
			power = power > max_power ? max_power : power;
			if (power > 20)
			{
				printf("");
			}
			int height_power = (height / max_power) * power;
			int pos_y_power = height - height_power;
			DrawRectangle(pos_x, pos_y + pos_y_power, width, height_power, RED);

			Vector2 dir = direction(main_ball->position, mouse_pos);
			Vector2 end_pos;
			end_pos.x = main_ball->position.x + diagonal * 2 * dir.x;
			end_pos.y = main_ball->position.y + diagonal * 2 * dir.y;
			DrawLineV(main_ball->position, end_pos, WHITE);
		}
		EndDrawing();
	}
	CloseWindow();
	free(balls);
	free(grid);
	return 0;
}