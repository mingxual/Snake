#include "Button.h"
#include "Texture.h"
#include "Font.h"

// x, y is the center of the button
Button::Button(int x, int y, int horizontalPadding, int verticalPadding)
	:mHorizontal(x), mVertical(y), mHorizontalPadding(horizontalPadding), mVerticalPadding(verticalPadding),
	 mNormalTexture(NULL),mHighlightTexture(NULL), mClickTexture(NULL), mText(""), mState(ButtonState::Normal)
{

}

Button::~Button()
{
	delete mNormalTexture;
	delete mHighlightTexture;
	delete mClickTexture;
}

void Button::Update(float deltaTime)
{
	bool containMouseCursor = ContainMouseCursor();

	if (mState == ButtonState::Click)
	{
		if (mOnClick)
		{
			mOnClick();
			mState = ButtonState::Normal;
		}
	}
	else if (containMouseCursor) // Highlight state
	{
		mState = ButtonState::HighLight;
	}
	else // Normal state
	{
		mState = ButtonState::Normal;
	}
}

void Button::SetOnClick(std::function<void()> OnClick)
{
	mOnClick = OnClick;
}

void Button::RenderToScreen(SDL_Renderer* render)
{
	SDL_Rect rect;
	rect.x = topleftCorner.x;
	rect.y = topleftCorner.y;
	rect.w = bottomrightCorner.x - topleftCorner.x;
	rect.h = bottomrightCorner.y - topleftCorner.y;

	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderFillRect(render, &rect);

	int x = topleftCorner.x + mHorizontalPadding;
	int y = topleftCorner.y + mVerticalPadding;
	if (mState == ButtonState::Normal)
	{
		mNormalTexture->RenderToScreen(render, Vector2(x, y));
	}
	else if (mState == ButtonState::HighLight)
	{
		mHighlightTexture->RenderToScreen(render, Vector2(x, y));
	}
	else
	{
		mClickTexture->RenderToScreen(render, Vector2(x, y));
	}
}

void Button::SetText(std::string text, Font* font)
{
	mText = text;

	mNormalTexture = font->RenderText(text);
	mHighlightTexture = font->RenderText(text, mHighlightColor);
	mClickTexture = font->RenderText(text, mClickColor);

	// Calculate the corner
	int topleftCorner_x = mHorizontal - mHorizontalPadding - mNormalTexture->GetWidth() >> 1;
	int topleftCorner_y = mVertical - mVerticalPadding - mNormalTexture->GetHeight() >> 1;
	int bottomrightCorner_x = topleftCorner_x + 2 * mHorizontalPadding + mNormalTexture->GetWidth();
	int bottomrightCorner_y = topleftCorner_y + 2 * mVerticalPadding + mNormalTexture->GetHeight();

	topleftCorner = Vector2(topleftCorner_x, topleftCorner_y);
	bottomrightCorner = Vector2(bottomrightCorner_x, bottomrightCorner_y);
}

bool Button::ContainMouseCursor()
{
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	// Normal state
	if (mouse_x < topleftCorner.x || mouse_x > bottomrightCorner.x ||
		mouse_y < topleftCorner.y || mouse_y > bottomrightCorner.y)
	{
		return false;
	}
	else // Highlight state
	{
		return true;
	}
}

void Button::CheckClickState()
{
	bool containMouseCursor = ContainMouseCursor();

	if (containMouseCursor)
	{
		mState = ButtonState::Click;
	}
}
