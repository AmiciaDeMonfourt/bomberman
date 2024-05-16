#include "button.hpp"
#include "../utils/texture_map.hpp"
#include "../consts.hpp"


Button::Button(const std::string& text, const SDL_FRect& hitBox, const SDL_Color& defaultColor, const SDL_Color& hoverColor) 
    : GameObject(hitBox), mDefaultTextColor(defaultColor), mHoverTextColor(hoverColor)
{
    mText = text;
    if(SDL_FRectEmpty(&hitBox))
        throw std::invalid_argument("Button not initialize:");
    
    mTextPos = {(int)hitBox.x, (int)hitBox.y};
}


void Button::Render(void) const
{
    
    if(mIsHover) RenderText(mText, mTextPos, mHoverTextColor);
    else RenderText(mText, mTextPos, mDefaultTextColor);
}

void Button::UserInput(const SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_MOUSEMOTION:
            mIsHover = CheckMousePos(event->button.x, event->button.y);
            break;
        
        case SDL_MOUSEBUTTONDOWN:
            if(CheckMousePos(event->button.x, event->button.y)) {
                mIsClick = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            mIsClick = false;
            break;
    }
}

bool Button::CheckMousePos(const int x, const int y) const
{
    return (x > mHitBox.x) && (y > mHitBox.y) && (x < mHitBox.x + mHitBox.w) && (y < mHitBox.y + mHitBox.h);
}
bool Button::GetIsHover(void) const
{
    return mIsHover;
}
bool Button::GetIsClick(void) const
{
    return mIsClick;
}

void Button::Reset(void)
{
    mIsClick = false;
    mIsHover = false;
}
