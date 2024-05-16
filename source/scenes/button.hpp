#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include "..\common.hpp"
#include "..\game_object.hpp"

class Button : public GameObject
{
    private:

        const SDL_Color& mDefaultTextColor;
        
        const SDL_Color& mHoverTextColor;

        bool mIsClick = false;

        bool mIsHover = false; 

        std::string mText;

        SDL_Point mTextPos;
 
    public:

        Button(const std::string& text, const SDL_FRect& hitBox, const SDL_Color& defaultColor, const SDL_Color& hoverColor);

        void Render(void) const;

        bool CheckMousePos(const int x, const int y) const;

        void UserInput(const SDL_Event* event) override;

        bool GetIsHover(void) const;

        bool GetIsClick(void) const;

        void Reset(void);
};

#endif