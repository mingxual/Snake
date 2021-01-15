#include "Game.h"
#include "Actor.h"
#include "Random.h"
#include <SDL/SDL_image.h>
#include <algorithm>
#include "Snake.h"
#include "Score.h"
#include "Font.h"
#include "Texture.h"
#include <string>
#include "Button.h"

Game::Game():window(NULL),render(NULL),running(true), mActors(NULL), mCellOccupied(NULL), mSnake(NULL), mScore(NULL),
			 mFont(NULL), mCurrentScore(NULL), mButton(NULL)
{ 	

}

bool Game::Initialize() 
{
	Random::Init();

	// Init SDL
	int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (result != 0) 
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create the window
	window = SDL_CreateWindow(
		"Snake",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		window_width,
		window_height,
		0
	);
	if (window == NULL) 
	{
		SDL_Log("Could not create window: %s", SDL_GetError());
		return false;
	}

	// Create the render
	render = SDL_CreateRenderer(
		window, 
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (render == NULL) 
	{
		SDL_Log("Could not create renderer: %s\n", SDL_GetError());
		return false;
	}

	// Initialize SDL Image
	int flags = IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags)
	{
		SDL_Log("IMG_Init: Failed to init required png support.\n");
		SDL_Log("IMG_Init: %s\n", IMG_GetError());
	}

	LoadData();
	time_count = SDL_GetTicks();

	return true;
}

void Game::RunLoop() 
{
	while (running) 
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() 
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type) 
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mButton->CheckClickState();
			break;
		}
	}
	// Create keyboard input
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) 
	{
		running = false;
	}

	// Loop over
	std::vector<Actor*> copyactors = mActors;
	for (auto actor : copyactors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame() 
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), time_count + 16));

	// Count the time
	float deltatime = (SDL_GetTicks() - time_count) / 1000.0f;
	if (deltatime > 0.033f) deltatime = 0.033f;
	time_count = SDL_GetTicks();

	if (mSnake == NULL)
	{
		mButton->Update(deltatime);
	}

	// Shallow copy actors vector
	std::vector<Actor*> copyactors = mActors;
	for (auto actor : copyactors) 
	{
		actor->Update(deltatime);
	}
	// Insert destroy-state actor to the list
	std::vector<Actor*> destroylist;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroylist.push_back(actor);
		}
	}
	// Destroy the actor
	for (auto actor : destroylist)
	{
		delete actor;
	}
}

void Game::GenerateOutput() 
{
	DrawLines();

	for (size_t i = 0; i < mActors.size(); ++i)
	{
		mActors[i]->RenderToGrid();
	}

	if (mSnake)
	{
		mCurrentScore = mFont->RenderText(std::to_string(mSnake->GetCount()));
		mCurrentScore->RenderToScreen(render, Vector2(10, 5));
	}

	if (mSnake == NULL)
	{
		mButton->RenderToScreen(render);
	}

	SDL_RenderPresent(render);
}

void Game::Shutdown() 
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::AddActor(Actor* actor) 
{
	mActors.push_back(actor);
}

void Game::RemoveActor(Actor* actor) 
{
	std::vector<Actor*>::iterator it = std::find(mActors.begin(), mActors.end(), actor);
	mActors.erase(it);
}

void Game::DrawLines()
{
	// Draw the background
	SDL_SetRenderDrawColor(render, grid_bg_color.r, grid_bg_color.g, grid_bg_color.b, grid_bg_color.a);
	SDL_RenderClear(render);

	// Draw the grid
	SDL_SetRenderDrawColor(render, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);
	for (int x = 0; x < window_width; x += cell_side_size) 
	{
		SDL_RenderDrawLine(render, x, 0, x, window_height);
	}

	for (int y = 0; y < window_height; y += cell_side_size) 
	{
		SDL_RenderDrawLine(render, 0, y, window_width, y);
	}
}

void Game::LoadData()
{
	mCellOccupied = new bool* [cell_row_size];
	for (int i = 0; i < cell_row_size; ++i)
	{
		mCellOccupied[i] = new bool[cell_column_size]();
	}

	for (int i = 0; i < cell_row_size; ++i)
	{
		for (int j = 0; j < cell_column_size; ++j)
		{
			mCellUnfilled.insert(i * 100 + j);
		}
	}

	mSnake = new Snake(this);
	mScore = new Score(this);
	mFont = new Font(this);
	mFont->Load("Assets/Inconsolata-Regular.ttf");

	mButton = new Button(window_width, window_height, 10, 5);
	mButton->SetText("Restart", mFont);
	mButton->SetOnClick([this] {
		mSnake = new Snake(this);
	});
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		RemoveActor(mActors.back());
	}
	for (auto pair : mTextures)
	{
		SDL_DestroyTexture(pair.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const char* filename)
{
	// Check if texture exists
	std::unordered_map<std::string, SDL_Texture*>::iterator it = mTextures.find(filename);
	if (it != mTextures.end())
	{
		return it->second;
	}

	// Import the file
	SDL_Surface* image = IMG_Load(filename);
	if (!image)
	{
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, image);
	SDL_FreeSurface(image);
	mTextures.insert(std::make_pair(filename, texture));
	
	return texture;
}

void Game::DeleteUnfilledCell(std::pair<int, int> cell)
{
	mCellUnfilled.insert(cell.first * 100 + cell.second);
}

void Game::AddUnfilledCell(std::pair<int, int> cell)
{
	mCellUnfilled.erase(cell.first * 100 + cell.second);
}

std::pair<int, int> Game::GetRandonUnfilledCell()
{
	int count = mCellUnfilled.size();
	int random = Random::GetIntRange(0, count);
	std::unordered_set<int>::iterator it = mCellUnfilled.begin();
	int i = 0;
	while (i++ < random)
	{
		++it;
	}
	
	int first = (*it) / 100, second = (*it) % 100;
	return std::pair<int, int>(first, second);
}

void Game::ClearSnake() 
{ 
	mSnake = NULL; 
}