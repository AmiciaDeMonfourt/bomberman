#ifndef __GAME_OBJECT_HPP__
#define __GAME_OBJECT_HPP__

#include "common.hpp"

enum Direction
{
    Up,
    Right,
    Down,
    Left,
    None
};  

class GameObject
{
    protected:

        SDL_FRect mHitBox;

        SDL_Rect mSpriteBox;

        std::string mTextureKey;

        bool needDelete = false;

    public:

        GameObject(void) {}

        GameObject(const SDL_FRect& hitBox);

        GameObject(const SDL_FRect& hitBox, const SDL_Rect& mSpriteBox);

        virtual ~GameObject(void) {}


        virtual void Render(void) const;

        virtual void UserInput(const SDL_Event* event);

        virtual void Update(void);

        void SetNeedDelete(bool state);

        bool GetNeedDelete(void) const;
};


#endif