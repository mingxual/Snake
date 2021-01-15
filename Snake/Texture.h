#pragma once
#include "SDL/SDL_render.h"
#include "Math.h"

class Texture
{
public:
	Texture(SDL_Texture* texture, int w, int h);
	~Texture();

	void RenderToScreen(SDL_Renderer* render, Vector2 offset = Vector2::Zero);
	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;

};

