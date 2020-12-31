#include "Snake.h"
#include "Game.h"

Snake::Snake(Game* game)
	:Actor(game), forwardSpeed(2.0f), forwardDistance(0.0f), count(2), direction(2)
{
	// Initial State
	Cell head(17, 12);
	Cell cell(17, 13);
	body.push_back(head);
	body.push_back(cell);

	bool** cell_occupied = GetGame()->GetCellOccupied();
	cell_occupied[17][12] = true;
	cell_occupied[17][13] = true;
}

void Snake::OnProcessInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN] && direction != 1)
	{
		direction = 0;
	}
	else if (!keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_DOWN] && direction != 0)
	{
		direction = 1;
	}
	else if (keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT] && direction != 3)
	{
		direction = 2;
	}
	else if (!keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RIGHT] && direction != 2)
	{
		direction = 3;
	}
}

void Snake::OnUpdate(float deltaTime)
{
	forwardDistance += forwardSpeed * deltaTime;
	if (forwardDistance >= 1.0f)
	{
		bool** cell_occupied = GetGame()->GetCellOccupied();
		cell_occupied[body[count - 1].row][body[count - 1].col] = false;

		for (int i = count - 1; i > 0; --i)
		{
			body[i].SetPosition(body[i - 1].row, body[i - 1].col);
		}

		switch (direction) 
		{
			case 0:
				body[0].row = body[0].row > 0 ? body[0].row - 1 : GetGame()->cell_row_size - 1;
				break;
			case 1:
				body[0].row = body[0].row < GetGame()->cell_row_size - 1 ? body[0].row + 1 : 0;
				break;
			case 2:
				if (body[0].col == 0)
				{
					int curr = GetGame()->cell_column_size;
					curr -= 1;
				}
				body[0].col = body[0].col > 0 ? body[0].col - 1 : GetGame()->cell_column_size - 1;
				break;
			case 3:
				body[0].col = body[0].col < GetGame()->cell_column_size - 1 ? body[0].col + 1 : 0;
				break;
		}

		if (cell_occupied[body[0].row][body[0].col])
		{
			SetState(ActorState::Destroy);
			return;
		}
		cell_occupied[body[0].row][body[0].col] = true;
		forwardDistance -= 1.0f;
	}
}

void Snake::RenderToGrid()
{
	int cell_side_size = GetGame()->cell_side_size;
	SDL_Renderer* render = GetGame()->GetRenderer();

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

	int x3 = 2 * x2 - x1;
	int y3 = 2 * y2 - y1;
	Cell next(x3, y3);
	body.push_back(next);
	++count;
}

