#include <stdio.h>
#include "Headers/game.h"
#include "Headers/utils.h"
#include "Headers/Objects/gamestate.h"
#include <stdbool.h>
#include <pthread.h>

void* gameLogic(GameState* state)
{
	while (true)
	{
		/*state->main_ball = &state->balls[*state->balls_count - 1];
		if (IsKeyPressed(KEY_G))
		{
			*(state->grid_show) = !*(state->grid_show);
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !*(state->shot) && !*(state->step))
		{
			Vector2 mouse_pos = GetMousePosition();
			if (distance(mouse_pos, state->main_ball->position) <= state->radius)
			{
				*(state->shot) = true;
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && *(state->shot))
		{
			Vector2 mouse_pos = GetMousePosition();
			*(state->shot) = false;
			Vector2 dir = direction(state->main_ball->position, mouse_pos);
			float coef = distance(state->main_ball->position, mouse_pos) / diagonal(state->win_size) / 2;
			float power = state->max_power * coef;
			power = power > state->max_power ? state->max_power : power;
			state->main_ball->velocity.x = dir.x * power;
			state->main_ball->velocity.y = dir.y * power;
			printf("X - %f Y - %f\n", dir.x * power, dir.y * power);
			*(state->step) = true;
		}*/
		moveBalls(state->balls, *state->balls_count, state->damping, *(state->delta));
		checkCells(state->grid, state->grid_size.x, state->grid_size.y, state->balls, *state->balls_count, state->radius);
		findCollisionGrid(state->balls, state->grid, state->grid_size.x, state->grid_size.y, state->radius);
		collisionScreen(state->balls, *state->balls_count, state->radius, state->win_size.x, state->win_size.y, *(state->delta));
		collisionHoles(state->balls, state->balls_count, state->radius, state->holes_positions, state->main_ball_pos);
		checkEndStep(state->balls, *state->balls_count, state->step);
		WaitTime(0.0001);
	}
	return NULL;
}
int main()
{
	int size = 16;
	int radius = 10;
	int balls_count = 0;
	float damping = 0.9999;
	float delta = 1;
	Vector2 win_size = { 800, 400 };
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
	Ball* balls = malloc(size * sizeof(Ball));

	generateGrid(grid_size.x, grid_size.y, grid);
	generateBalls(balls, win_size, radius, &balls_count);

	Vector2  main_ball_pos = { round(win_size.x / 3.5), win_size.y / 2 };
	balls[balls_count].position = main_ball_pos;
	balls[balls_count].velocity = (Vector2){ 0 ,0 };
	balls[balls_count].type = Main;
	snprintf(balls[balls_count].number, sizeof(balls[balls_count].number), "%d", balls_count + 1);

	Ball* main_ball = &balls[balls_count++];

	GameState state =
	{
		.balls = balls,
		.grid = grid,
		.main_ball = main_ball,
		.main_ball_pos = main_ball_pos,
		.win_size = win_size,
		.grid_size = grid_size,
		.holes_positions = holes_positions,
		.balls_count = &balls_count,
		.grid_show = &grid_show,
		.shot = &shot,
		.step = &step,
		.radius = radius,
		.max_power = max_power,
		.damping = damping,
		.delta = &delta
	};
	InitWindow(win_size.x, win_size.y, "Billiards");
	SetTargetFPS(60);
	SetWindowState(FLAG_VSYNC_HINT);

	pthread_t logicThread;
	pthread_create(&logicThread, NULL, gameLogic, &state);

	float previous_time = GetTime();
	float current_time = 0;
	float delta_time = 0;

	while (!WindowShouldClose())
	{
		current_time = GetTime();
		delta_time = current_time - previous_time;
		previous_time = current_time;

		delta = delta_time;

		main_ball = &balls[balls_count - 1];
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
			float coef = distance(main_ball->position, mouse_pos) / diagonal(win_size) / 2;
			float power = max_power * coef;
			power = power > max_power ? max_power : power;
			main_ball->velocity.x = dir.x * power;
			main_ball->velocity.y = dir.y * power;
			step = true;
		}

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
			float coef = distance(main_ball->position, mouse_pos) / (diagonal(win_size) / 2);
			float power = max_power * coef;
			power = power > max_power ? max_power : power;
			int height_power = (height / max_power) * power;
			int pos_y_power = height - height_power;
			DrawRectangle(pos_x, pos_y + pos_y_power, width, height_power, RED);

			Vector2 dir = direction(main_ball->position, mouse_pos);
			Vector2 end_pos = { main_ball->position.x + diagonal(win_size) * dir.x, main_ball->position.y + diagonal(win_size) * dir.y };
			DrawLineV(main_ball->position, end_pos, WHITE);
		}
		EndDrawing();
	}
	CloseWindow();
	free(balls);
	free(grid);
	return 0;
}