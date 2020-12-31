#pragma once

#include "SDL/SDL.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Game {
public:
	Game();
	bool Initialize();
	void Shutdown();
	void RunLoop();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	SDL_Texture* GetTexture(const char* filename);
	SDL_Renderer* GetRenderer() { return render; }
	bool** GetCellOccupied() { return cell_occupied; }
	class Snake* GetSnake() { return snake; }
	void DeleteUnfilledCell(std::pair<int, int> cell);
	void AddUnfilledCell(std::pair<int, int> cell);
	std::pair<int, int> GetRandonUnfilledCell();

	// The setting should be a rectangle;
	int cell_row_size = 25;
	int cell_column_size = 35;
	int cell_side_size = 30;

	int window_width = cell_side_size * cell_column_size + 1;
	int window_height = cell_side_size * cell_row_size + 1;

private:
	void DrawLines();

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* window;
	SDL_Renderer* render;

	// Color of bg
	SDL_Color grid_bg_color = { 25, 25, 25, 255 };
	// Color of line
	SDL_Color grid_line_color = { 45, 45, 45, 255 };

	// Variable to control the exit of game loop
	bool running;

	Uint32 time_count = 0;

	std::vector<class Actor*> mActors;
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	std::unordered_set<int> cell_unfilled;
	bool** cell_occupied;
	
	void LoadData();
	void UnloadData();

	class Snake* snake;
};