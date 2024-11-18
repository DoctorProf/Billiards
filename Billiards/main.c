#include <stdio.h>
#include "Headers/game.h"
#include "Headers/utils.h"
#include <stdbool.h>

int main()
{
	float damping = 0.99;
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
		fprintf(stderr, "Ошибка выделения памяти\n");
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
				printf(" %d ", grid[i][j].balls_index[k]);
			}
			printf("\n");
		}
	}
	bool shot = false;
	bool round = false;
	Vector2 current_mouse_pos = { 0, 0 };
	float max_power = 10;

	while (!WindowShouldClose())
	{
		current_mouse_pos = GetMousePosition();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !shot && !round)
		{
			if (distance(current_mouse_pos, main_ball->position) <= radius)
			{
				shot = true;
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && shot)
		{
			shot = false;
			Vector2 dir = direction(main_ball->position, current_mouse_pos);
			float power = distance(main_ball->position, current_mouse_pos) / max_power;
			power = power > 10 ? max_power : power;
			main_ball->velocity.x = dir.x * power;
			main_ball->velocity.y = dir.y * power;
			round = true;
		}
		moveBalls(balls, balls_count, damping);
		//checkCells(grid, width_grid, height_grid, balls, balls_count, radius);
		collisionBalls(balls, balls_count, radius);
		//findCollisionGrid(balls, grid, width_grid, height_grid, radius);
		collisionScreen(balls, balls_count, radius, screen_width, screen_height);
		checkEndRound(balls, balls_count, &round);
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
			int dist = distance(current_mouse_pos, main_ball->position) / max_power;
			dist = dist > 10 ? max_power : dist;
			int height_power = height / max_power * dist;
			int pos_y_power = height - height_power;
			DrawRectangle(pos_x, pos_y + pos_y_power, width, height_power, RED);

			Vector2 dir = direction(main_ball->position, current_mouse_pos);
			Vector2 end_pos;
			end_pos.x = main_ball->position.x + 100 * dir.x;
			end_pos.y = main_ball->position.y + 100 * dir.y;
			DrawLineV(main_ball->position, end_pos, WHITE);
		}
		EndDrawing();
	}
	CloseWindow();
	free(balls);
	free(grid);
	return 0;
}