#pragma once
#include "Actor.h"
#include <SDL/SDL_pixels.h>
#include <vector>

class Score : public Actor
{
public:
	Score(class Game* game);
	void OnUpdate(float deltaTime);
	void RenderToGrid();
	
	struct Cell {
		int row;
		int col;

		Cell(int i, int j) : row(i), col(j) {}
		void SetPosition(int i, int j) { row = i; col = j; }
	};

private:
	std::vector<Cell> points;
	
	SDL_Color score_color = { 200, 0, 0, 255 };

};

