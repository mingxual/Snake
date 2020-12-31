#include "Score.h"
#include "Game.h"
#include "Snake.h"

Score::Score(Game* game)
	:Actor(game)
{
	Cell point(-1, -1);
	points.push_back(point);
	ResetPointPos();
}

void Score::OnUpdate(float deltaTime)
{
	Snake* snake = mGame->GetSnake();
	if (snake == NULL)
	{
		return;
	}
	std::pair<int, int> snake_head_pos = snake->GetHeadPos();

	int index = -1;
	for (size_t i = 0; i < points.size(); ++i)
	{
		if (points[i].row == snake_head_pos.first && points[i].col == snake_head_pos.second)
		{
			index = i;
			break;
		}
	}
	if (index > -1)
	{
		// points.erase(points.begin() + index);
		ResetPointPos();
		mGame->GetSnake()->IncrementLength();
	}
}

void Score::RenderToGrid()
{
	int cell_side_size = mGame->cell_side_size;
	SDL_Renderer* render = mGame->GetRenderer();

	SDL_SetRenderDrawColor(render, score_color.r, score_color.g, score_color.b, score_color.a);

	for (size_t i = 0; i < points.size(); ++i)
	{
		SDL_Rect cell;
		cell.x = points[i].col * cell_side_size;
		cell.y = points[i].row * cell_side_size;
		cell.w = cell_side_size;
		cell.h = cell_side_size;

		SDL_RenderFillRect(render, &cell);
	}
}

void Score::ResetPointPos()
{
	std::pair<int, int> pos = mGame->GetRandonUnfilledCell();
	points[0].SetPosition(pos.first, pos.second);
}