#include "Snake.h"
#include "Game.h"
#include <algorithm>

Snake::Snake(Game* game)
	:Actor(game), mForwardSpeed(2.0f), mForwardDistance(0.0f), mCount(2), mDirection(2), mPrevMoved(false)
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
	if (mPrevMoved)
	{
		if (keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN] && mDirection != 1)
		{
			mDirection = 0;
			mPrevMoved = false;
		}
		else if (!keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_DOWN] && mDirection != 0)
		{
			mDirection = 1;
			mPrevMoved = false;
		}
		else if (keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT] && mDirection != 3)
		{
			mDirection = 2;
			mPrevMoved = false;
		}
		else if (!keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RIGHT] && mDirection != 2)
		{
			mDirection = 3;
			mPrevMoved = false;
		}
	}
}

void Snake::OnUpdate(float deltaTime)
{
	mForwardDistance += mForwardSpeed * deltaTime;
	if (mForwardDistance >= 1.0f)
	{
		mPrevMoved = true;

		bool** cell_occupied = mGame->GetCellOccupied();
		int last_x = body[mCount - 1].row, last_y = body[mCount - 1].col;

		cell_occupied[last_x][last_y] = false;
		mGame->AddUnfilledCell(std::pair<int, int>(last_x, last_y));

		for (int i = mCount - 1; i > 0; --i)
		{
			body[i].SetPosition(body[i - 1].row, body[i - 1].col);
		}

		switch (mDirection) 
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
			mGame->ClearSnake();
			return;
		}
		cell_occupied[body[0].row][body[0].col] = true;
		mGame->DeleteUnfilledCell(std::pair<int, int>(body[0].row, body[0].col));

		mForwardDistance -= 1.0f;
	}
}

void Snake::RenderToGrid()
{
	int cell_side_size = mGame->cell_side_size;
	SDL_Renderer* render = mGame->GetRenderer();

	SDL_SetRenderDrawColor(render, snake_body_color.r, snake_body_color.g, snake_body_color.b, snake_body_color.a);

	for (int i = 0; i < mCount; ++i)
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
	int x1 = body[mCount - 2].row;
	int y1 = body[mCount - 2].col;
	int x2 = body[mCount - 1].row;
	int y2 = body[mCount - 1].col;

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
	++mCount;

	if (mCount % 10 == 0)
	{
		mForwardSpeed *= 1.1f;
		mForwardSpeed = std::min(mForwardSpeed, 30.0f);
	}

	bool** cell_occupied = mGame->GetCellOccupied();
	cell_occupied[x3][y3] = true;
	mGame->DeleteUnfilledCell(std::pair<int, int>(x3, y3));
}

std::pair<int, int> Snake::GetHeadPos()
{
	return std::pair<int, int>(body[0].row, body[0].col); 
}
