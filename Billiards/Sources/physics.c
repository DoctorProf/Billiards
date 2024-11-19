#include "../Headers/physics.h"

void checkCells(Cell** grid, int width_grid, int height_grid, Ball* balls, int balls_count, int radius)
{
	for (int i = 0; i < height_grid; ++i)
	{
		for (int j = 0; j < width_grid; ++j)
		{
			if (grid[i][j].balls_index == NULL)
			{
				continue;
			}
			else
			{
				free(grid[i][j].balls_index);
				grid[i][j].balls_index = NULL;
				grid[i][j].count_balls = 0;
			}
		}
	}
	for (int i = 0; i < balls_count; ++i)
	{
		int x = balls[i].position.x / (2 * radius);
		int y = balls[i].position.y / (2 * radius);
		if (x >= 0 && x < width_grid && y >= 0 && y < height_grid)
		{
			grid[y][x].balls_index = (int*)realloc(grid[y][x].balls_index, (grid[y][x].count_balls + 1) * sizeof(int));
			grid[y][x].balls_index[grid[y][x].count_balls] = i;
			grid[y][x].count_balls += 1;
		}
	}
}
void moveBalls(Ball* balls, int balls_count, float damping)
{
	for (int i = 0; i < balls_count; ++i)
	{
		if (lengthVector(balls[i].velocity) == 0)
		{
			continue;
		}
		if (lengthVector(balls[i].velocity) < 0.2)
		{
			balls[i].velocity.x = 0;
			balls[i].velocity.y = 0;
		}
		else
		{
			balls[i].velocity.x = balls[i].velocity.x * damping;
			balls[i].velocity.y = balls[i].velocity.y * damping;
		}
		balls[i].position.x += balls[i].velocity.x;
		balls[i].position.y += balls[i].velocity.y;
	}
}
void collisionScreen(Ball* balls, int balls_count, int radius, int screen_width, int screen_height)
{
	for (int i = 0; i < balls_count; ++i)
	{
		bool col_v = false;
		bool col_h = false;
		Vector2 velocity = normalizeVector(balls[i].velocity);
		Vector2 pos_check = balls[i].position;
		pos_check.x += velocity.x * radius;
		pos_check.y += velocity.y * radius;
		Vector2 v_normal = { 0, 1 };
		Vector2 h_hormal = { 1, 0 };
		float value = 0;
		if (pos_check.y < 0)
		{
			col_v = true;
			value = -pos_check.y;
		}
		if (pos_check.y > screen_height)
		{
			col_v = true;
			value = pos_check.y - screen_height;
		}
		if (pos_check.x < 0)
		{
			col_h = true;
			value = -pos_check.x;
		}
		if (pos_check.x > screen_width)
		{
			col_h = true;
			value = pos_check.x - screen_width;
		}
		if (col_v || col_h)
		{
			balls[i].position.x += -velocity.x * ceil(value);
			balls[i].position.y += -velocity.y * ceil(value);
		}
		if (col_v)
		{
			Vector2 reflect_velocity = reflectVector(balls[i].velocity, v_normal);
			balls[i].velocity.x = reflect_velocity.x;
			balls[i].velocity.y = reflect_velocity.y;
		}
		if (col_h)
		{
			Vector2 reflect_velocity = reflectVector(balls[i].velocity, h_hormal);
			balls[i].velocity.x = reflect_velocity.x;
			balls[i].velocity.y = reflect_velocity.y;
		}
	}
}
bool collide(Vector2 pos_1, Vector2 pos_2, int radius)
{
	if (pos_1.x == pos_2.x && pos_1.y == pos_2.y) return false;
	return distance(pos_1, pos_2) < 2.f * radius;
}
//void solveCollision(Ball* current_ball, Ball* other_ball)
//{
//	Vector2 normal = subtractVectors(current_ball->position, other_ball->position);
//	normal = normalizeVector(normal);
//
//	Vector2 relative_velocity = subtractVectors(current_ball->velocity, other_ball->velocity);
//
//	float dot = dotProduct(relative_velocity, normal);
//
//	Vector2 impulseVector = scaleVector(normal, dot);
//	current_ball->velocity = subtractVectors(current_ball->velocity, scaleVector(impulseVector, 0.9f));
//	other_ball->velocity = addVectors(other_ball->velocity, impulseVector);
//}
void moveBall(Ball* ball, Vector2 step)
{
	ball->position.x += step.x;
	ball->position.y += step.y;
}
void solveCollision(Ball* current_ball, Ball* other_ball, int radius)
{
	Vector2 dir = direction(current_ball->position, other_ball->position);
	float dist = distance(current_ball->position, other_ball->position);

	float minDistance = radius * 2.f;
	float c = (minDistance - dist);
	Vector2 p = { 0, 0 };
	p.x = -c * dir.x * 0.5f;
	p.y = -c * dir.y * 0.5f;
	moveBall(other_ball, p);
	p.x = -p.x;
	p.y = -p.y;
	moveBall(current_ball, p);
	Vector2 normal = subtractVectors(current_ball->position, other_ball->position);
	normal = normalizeVector(normal);

	Vector2 relative_velocity = subtractVectors(current_ball->velocity, other_ball->velocity);

	float dot = dotProduct(relative_velocity, normal);

	Vector2 impulseVector = scaleVector(normal, dot);
	current_ball->velocity = subtractVectors(current_ball->velocity, scaleVector(impulseVector, 0.9f));
	other_ball->velocity = addVectors(other_ball->velocity, impulseVector);
}
void checkCellsCollision(Ball* balls, Cell* current_cell, Cell* other_cell, int radius)
{
	for (int i = 0; i < current_cell->count_balls; ++i)
	{
		for (int j = 0; j < other_cell->count_balls; ++j)
		{
			Ball* current_ball = &balls[current_cell->balls_index[i]];
			Ball* other_ball = &balls[other_cell->balls_index[j]];
			if (collide(current_ball->position, other_ball->position, radius))
			{
				solveCollision(current_ball, other_ball, radius);
			}
		}
	}
}
void findCollisionGrid(Ball* balls, Cell** grid, int width_grid, int height_grid, int radius)
{
	for (int i = 0; i < height_grid; ++i)
	{
		for (int j = 0; j < width_grid; ++j)
		{
			Cell* current_cell = &grid[i][j];
			for (int di = -1; di <= 1; ++di)
			{
				for (int dj = -1; dj <= 1; ++dj)
				{
					if ((i + di < 0 || i + di > height_grid - 1) || (j + dj < 0 || j + dj > width_grid - 1)) continue;
					Cell* other_cell = &grid[i + di][j + dj];
					checkCellsCollision(balls, current_cell, other_cell, radius);
				}
			}
		}
	}
}
void collisionBalls(Ball* balls, int balls_count, int radius)
{
	for (int i = 0; i < balls_count; ++i)
	{
		Ball* current_ball = &balls[i];
		for (int j = 0; j < balls_count; ++j)
		{
			if (i != j)
			{
				Ball* other_ball = &balls[j];
				if (collide(current_ball->position, other_ball->position, radius))
				{
					Vector2 normal = subtractVectors(current_ball->position, other_ball->position);
					normal = normalizeVector(normal);

					Vector2 relative_velocity = subtractVectors(current_ball->velocity, other_ball->velocity);

					float dot = dotProduct(relative_velocity, normal);

					Vector2 impulseVector = scaleVector(normal, dot);
					current_ball->velocity = subtractVectors(current_ball->velocity, scaleVector(impulseVector, 0.9f));
					other_ball->velocity = addVectors(other_ball->velocity, impulseVector);
				}
			}
		}
	}
}