#include "Score.h"
#include "Game.h"
#include "Snake.h"

Score::Score(Game* game)
	:Actor(game)
{
	Cell first(0, 0);
	Cell second(5, 5);
	points.push_back(first);
	points.push_back(second);
}

void Score::OnUpdate(float deltaTime)
{
	std::pair<int, int> snake_head_pos = GetGame()->GetSnake()->GetHeadPos();

	int index = -1;
	for (int i = 0; i < points.size(); ++i)
	{
		if (points[i].row == snake_head_pos.first && points[i].col == snake_head_pos.second)
		{
			index = i;
			break;
		}
	}
	if (index > -1)
	{
		points.erase(points.begin() + index);
		GetGame()->GetSnake()->IncrementLength();
	}
}

void Score::RenderToGrid()
{
	int cell_side_size = GetGame()->cell_side_size;
	SDL_Renderer* render = GetGame()->GetRenderer();

	SDL_SetRenderDrawColor(render, score_color.r, score_color.g, score_color.b, score_color.a);

	for (int i = 0; i < points.size(); ++i)
	{
		SDL_Rect cell;
		cell.x = points[i].col * cell_side_size;
		cell.y = points[i].row * cell_side_size;
		cell.w = cell_side_size;
		cell.h = cell_side_size;

		SDL_RenderFillRect(render, &cell);
	}
}