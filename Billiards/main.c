#include <stdio.h>
#include "Headers/game.h"
#include "Headers/utils.h"
#include <stdbool.h>

int main()
{
	float damping = 0.99;
	int screen_width = 800;
	int screen_height = 400;
	int diagonal = sqrtf(powf(screen_width, 2) + powf(screen_height, 2)) / 2;
	int radius = 10;

	Vector2 holes_positions[6] =
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
		fprintf(stderr, "Ошибка выделения памяти\n");
		free(balls);
		return 1;
	}
	int balls_count = generateBalls(balls, screen_width, screen_height, radius);
	Vector2  main_ball_pos = { 0, 0 };
	main_ball_pos.x = round(screen_width / 3.5);
	main_ball_pos.y = screen_height / 2;
	snprintf(balls[balls_count].number, sizeof(balls[balls_count].number), "%d", balls_count + 1);
	balls[balls_count].position = main_ball_pos;
	balls[balls_count].velocity = (Vector2){ 0 ,0 };
	balls[balls_count].type = Main;

	Ball* main_ball = &balls[balls_count++];

	generateGrid(width_grid, height_grid, grid);
	InitWindow(screen_width, screen_height, "Billiards");
	SetTargetFPS(1000);
	//SetWindowState(FLAG_VSYNC_HINT);
	bool grid_show = false;
	bool shot = false;
	bool round = false;
	float max_power = 30;

	while (!WindowShouldClose())
	{
		main_ball = &balls[balls_count - 1];
		if (IsKeyPressed(KEY_G))
		{
			grid_show = !grid_show;
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !shot && !round)
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
			round = true;
		}
		moveBalls(balls, balls_count, damping);
		checkCells(grid, width_grid, height_grid, balls, balls_count, radius);
		findCollisionGrid(balls, grid, width_grid, height_grid, radius);
		collisionScreen(balls, balls_count, radius, screen_width, screen_height);
		collisionHoles(balls, &balls_count, radius, holes_positions, main_ball_pos);
		checkEndRound(balls, balls_count, &round);

		BeginDrawing();
		ClearBackground(DARKGREEN);
		if (grid_show)
		{
			for (int i = 0; i < height_grid; ++i)
			{
				for (int j = 0; j < width_grid; ++j)
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
			int width = screen_width / 40;
			int height = screen_height / 5;
			int pos_x = 10;
			int pos_y = screen_height / 2 - height / 2;
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