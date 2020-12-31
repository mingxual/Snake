#include "Snake.h"
#include "Game.h"
#include <algorithm>

Snake::Snake(Game* game)
	:Actor(game), forwardSpeed(2.0f), forwardDistance(0.0f), count(2), direction(2), prev_moved(false)
{
	// Initial State
	Cell head(17, 12);
	Cell cell(17, 13);
	body.push_back(head);
	body.push_back(cell);

	bool** cell_occupied = mGame->GetCellOccupied();
	cell_occupied[17][12] = true;
	cell_occupied[17][13] = true;

	mGame->DeleteUnfilledCell(std::pair<int, int>(17, 12));
	mGame->DeleteUnfilledCell(std::pair<int, int>(17, 13));
}

void Snake::OnProcessInput(const Uint8* keyState)
{
	if (prev_moved)
	{
		if (keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN] && direction != 1)
		{
			direction = 0;
			prev_moved = false;
		}
		else if (!keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_DOWN] && direction != 0)
		{
			direction = 1;
			prev_moved = false;
		}
		else if (keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT] && direction != 3)
		{
			direction = 2;
			prev_moved = false;
		}
		else if (!keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RIGHT] && direction != 2)
		{
			direction = 3;
			prev_moved = false;
		}
	}
}

void Snake::OnUpdate(float deltaTime)
{
	forwardDistance += forwardSpeed * deltaTime;
	if (forwardDistance >= 1.0f)
	{
		prev_moved = true;

		bool** cell_occupied = mGame->GetCellOccupied();
		int last_x = body[count - 1].row, last_y = body[count - 1].col;

		cell_occupied[last_x][last_y] = false;
		mGame->AddUnfilledCell(std::pair<int, int>(last_x, last_y));

		for (int i = count - 1; i > 0; --i)
		{
			body[i].SetPosition(body[i - 1].row, body[i - 1].col);
		}

		switch (direction) 
		{
			case 0:
				body[0].row = body[0].row > 0 ? body[0].row - 1 : mGame->cell_row_size - 1;
				break;
			case 1:
				body[0].row = body[0].row < mGame->cell_row_size - 1 ? body[0].row + 1 : 0;
				break;
			case 2:
				if (body[0].col == 0)
				{
					int curr = mGame->cell_column_size;
					curr -= 1;
				}
				body[0].col = body[0].col > 0 ? body[0].col - 1 : mGame->cell_column_size - 1;
				break;
			case 3:
				body[0].col = body[0].col < mGame->cell_column_size - 1 ? body[0].col + 1 : 0;
				break;
		}

		if (cell_occupied[body[0].row][body[0].col])
		{
			SetState(ActorState::Destroy);
			return;
		}
		cell_occupied[body[0].row][body[0].col] = true;
		mGame->DeleteUnfilledCell(std::pair<int, int>(body[0].row, body[0].col));

		forwardDistance -= 1.0f;
	}
}

void Snake::RenderToGrid()
{
	int cell_side_size = mGame->cell_side_size;
	SDL_Renderer* render = mGame->GetRenderer();

	SDL_SetRenderDrawColor(render, snake_body_color.r, snake_body_color.g, snake_body_color.b, snake_body_color.a);

	for (int i = 0; i < count; ++i)
	{
		SDL_Rect cell;
		cell.x = body[i].col * cell_side_size;
		cell.y = body[i].row * cell_side_size;
		cell.w = cell_side_size;
		cell.h = cell_side_size;

		SDL_RenderFillRect(render, &cell);
	}
}

void Snake::IncrementLength()
{
	int x1 = body[count - 2].row;
	int y1 = body[count - 2].col;
	int x2 = body[count - 1].row;
	int y2 = body[count - 1].col;

	// Boundary check
	int x3 = 2 * x2 - x1;
	if (x3 < 0)
	{
		x3 = mGame->cell_row_size - 1;
	}
	else if (x3 >= mGame->cell_row_size)
	{
		x3 = 0;
	}

	int y3 = 2 * y2 - y1;
	if (y3 < 0)
	{
		y3 = mGame->cell_column_size - 1;
	}
	else if (y3 >= mGame->cell_column_size)
	{
		y3 = 0;
	}

	// Create a cell to the tail of the snake
	Cell next(x3, y3);
	body.push_back(next);
	++count;

	if (count % 10 == 0)
	{
		forwardSpeed *= 1.1f;
		forwardSpeed = std::min(forwardSpeed, 30.0f);
	}

	bool** cell_occupied = mGame->GetCellOccupied();
	cell_occupied[x3][y3] = true;
	mGame->DeleteUnfilledCell(std::pair<int, int>(x3, y3));
}

