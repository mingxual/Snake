#pragma once
#include "Actor.h"
#include <SDL/SDL_pixels.h>
#include <vector>

class Snake : public Actor
{
public:
	Snake(class Game* game);
	void OnProcessInput(const Uint8* keyState);
	void OnUpdate(float deltaTime);
	void RenderToGrid();
	std::pair<int, int> GetHeadPos() { return std::pair<int, int>(body[0].row, body[0].col); }
	void IncrementLength();

	struct Cell {
		int row;
		int col;

		Cell(int i, int j) : row(i), col(j) {}
		void SetPosition(int i, int j) { row = i; col = j; }
	};

private:
	float forwardSpeed;
	float forwardDistance;
	int count;
	std::vector<Cell> body;
	
	// 0 up, 1 down, 2 left, 3 right
	int direction;

	SDL_Color snake_head_color = { 160, 160, 160, 255 };
	SDL_Color snake_body_color = { 160, 160, 160, 255 };
};

