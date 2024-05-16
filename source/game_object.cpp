#include "game_object.hpp"
#include "utils/texture_map.hpp"

GameObject::    GameObject(const SDL_FRect& hitBox) 
    : mHitBox(hitBox)
{
    if(SDL_FRectEmpty(&hitBox))
        throw std::invalid_argument("GameObject: Invalid hitBox argument");
}


GameObject::GameObject(const SDL_FRect& hitBox, const SDL_Rect& spriteBox)
    : mHitBox(hitBox), mSpriteBox(spriteBox)
{
    if(SDL_FRectEmpty(&hitBox))
        throw std::invalid_argument("GameObject: Invalid hitBox argument");

    if(SDL_RectEmpty(&spriteBox))
        throw std::invalid_argument("GameObject: Invalid spriteBox argument");
}


void GameObject::Render(void) const
{
    TextureMap::Get(mTextureKey)->Render(mHitBox, mSpriteBox);
}

void GameObject::UserInput(const SDL_Event *event){}

void GameObject::Update(void){}

void GameObject::SetNeedDelete(bool state) { needDelete = state; }

bool GameObject::GetNeedDelete(void) const { return needDelete; }