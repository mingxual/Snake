#include "Texture.h"
#include "Game.h"

Texture::Texture(SDL_Texture* texture, int w, int h)
	:mTexture(texture), mWidth(w), mHeight(h)
{
	
}

Texture::~Texture()
{
	SDL_DestroyTexture(mTexture);
}

void Texture::RenderToScreen(SDL_Renderer* render, Vector2 offset)
{
	SDL_Rect rect;
	rect.x = (int)offset.x;
	rect.y = (int)offset.y;
	rect.w = mWidth;
	rect.h = mHeight;

	SDL_RenderCopy(render, mTexture, NULL, &rect);
}
