#pragma once
#include "Math.h"
#include <functional>
#include <string>
#include "SDL/SDL_render.h"

enum class ButtonState
{
	Normal,
	HighLight,
	Click
};

class Button
{
public:
	Button(int x, int y, int horizontalPadding, int verticalPadding);
	~Button();

	void Update(float deltaTime);
	void SetOnClick(std::function<void()> OnClick);
	void RenderToScreen(SDL_Renderer* render);
	void SetText(std::string text, class Font* font);
	bool ContainMouseCursor();
	void CheckClickState();

private:
	int mHorizontal;
	int mVertical;
	int mHorizontalPadding;
	int mVerticalPadding;
	
	const Vector3 mNormalColor = Color::Black;
	const Vector3 mHighlightColor = Color::Blue;
	const Vector3 mClickColor = Color::Green;

	std::function<void()> mOnClick;

	Vector2 topleftCorner;
	Vector2 bottomrightCorner;

	class Texture* mNormalTexture;
	class Texture* mHighlightTexture;
	class Texture* mClickTexture;

	std::string mText;

	ButtonState mState;
};

