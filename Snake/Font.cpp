#include "Font.h"
#include <vector>
#include "Game.h"
#include "Texture.h"

Font::Font(Game* game)
	:mGame(game)
{
	
}

Font::~Font()
{
	
}

bool Font::Load(const std::string& fileName)
{
	TTF_Init();

	// We support these font sizes
	std::vector<int> fontSizes = {
		12, 18,
		24, 36, 48, 60, 72
	};
	
	for (auto& size : fontSizes)
	{
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
			return false;
		}
		mFontData.emplace(size, font);
	}
	return true;
}

void Font::Unload()
{
	for (auto& font : mFontData)
	{
		TTF_CloseFont(font.second);
	}

	TTF_Quit();
}

Texture* Font::RenderText(const std::string& text,
						      const Vector3& color /*= Color::Red*/,
						      int pointSize /*= 24*/)
{
	Texture* texture = nullptr;
	
	// Convert to SDL_Color
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;
	
	// Find the font data for this point size
	auto iter = mFontData.find(pointSize);
	if (iter != mFontData.end())
	{
		TTF_Font* font = iter->second;
		// Draw this to a surface (blended for alpha)
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), sdlColor);
		if (surf != nullptr)
		{
			// Convert from surface to texture
			SDL_Texture* _texture = SDL_CreateTextureFromSurface(mGame->GetRenderer(), surf);
			texture = new Texture(_texture, surf->w, surf->h);

			SDL_FreeSurface(surf);
		}
	}
	else
	{
		SDL_Log("Point size %d is unsupported", pointSize);
	}

	return texture;
}
