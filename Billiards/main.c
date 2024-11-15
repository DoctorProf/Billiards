#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Headers/Ball.h"

int main()
{
	const float damping = 0.1;
	const int screen_width = 800;
	const int screen_height = 400;
	const int fps = 120;
	int radius = 10;
	int holes_positions[6][2] = { {radius, radius},
								  {400 + radius, radius},
								  {800 - radius, radius},
								  {radius, 400 - radius},
								  {400, 400 - radius},
								  {800 - radius, 400 - radius}
	};
	const int size = 16;
	Ball* balls = (Ball*)malloc(size * sizeof(Ball));

	float start_x = round(screen_width / 1.5);
	float start_y = screen_height / 2;
	int balls_count = 0;
	for (int i = 0; i < 5; ++i)
	{
		start_y -= radius;
		start_x += 2 * radius;
		for (int j = 0; j <= i; ++j)
		{
			Ball ball;
			ball.position.x = start_x;
			ball.position.y = start_y + 2 * (radius + 1) * j;
			ball.velocity.x = 0;
			ball.velocity.y = 0;
			ball.type = Base;
			balls[balls_count] = ball;
			++balls_count;
		}
	}
	Ball play_ball;
	play_ball.position.x = (float)round(screen_width / 2.5);
	play_ball.position.y = screen_height / 2;
	play_ball.velocity.x = 0;
	play_ball.velocity.y = 0;
	play_ball.type = Main;
	balls[balls_count] = play_ball;
	++balls_count;
	InitWindow(screen_width, screen_height, "Billiards");
	SetTargetFPS(fps);

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
		}
		EndDrawing();
	}
	CloseWindow();
	free(balls);
	return 0;
}