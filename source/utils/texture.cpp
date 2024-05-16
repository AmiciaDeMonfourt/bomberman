#include "texture.hpp"


Texture::Texture(const std::string &fileName, SDL_Renderer* renderer)
{
    if(fileName == "") {
        throw std::invalid_argument(std::string("Texture: fileName == \"\""));
    }
    
    if(renderer == nullptr) {
        throw std::invalid_argument(std::string("Texture: renderer == nullptr"));
    }
    mRenderer = renderer;

    SDL_Surface* imgSurface = IMG_Load(fileName.c_str());
    if(imgSurface == nullptr){
        throw std::runtime_error(std::string("Texture: invalid fileName: ") + SDL_GetError());
    }

    mTexture = SDL_CreateTextureFromSurface(renderer, imgSurface);
    if(mTexture == nullptr) {
        throw std::runtime_error(std::string("Texture: texture doesn't create:") + SDL_GetError() );
    }
}   



void Texture::CleanUp(void)
{
    SDL_DestroyTexture(mTexture);
}

void Texture::Render(const SDL_FRect &hitBox, const SDL_Rect &spriteBox) const
{
    SDL_Rect intHitBox = { static_cast<int>(hitBox.x), static_cast<int>(hitBox.y), static_cast<int>(hitBox.w), static_cast<int>(hitBox.h) };
    
    SDL_RenderCopy(mRenderer, mTexture, &spriteBox, &intHitBox);
}


void Texture::Render(const SDL_FRect& hitBox) const
{
    SDL_Rect intHitBox = { static_cast<int>(hitBox.x), static_cast<int>(hitBox.y), static_cast<int>(hitBox.w), static_cast<int>(hitBox.h) };
    
    SDL_RenderCopy(mRenderer, mTexture, nullptr, &intHitBox);
}


void Texture::Render(const SDL_FRect& hitBox, double angle) const
{
    SDL_Rect intHitBox = { static_cast<int>(hitBox.x), static_cast<int>(hitBox.y), static_cast<int>(hitBox.w), static_cast<int>(hitBox.h) };
    SDL_Point centered = { static_cast<int>(hitBox.w / 2), static_cast<int>(hitBox.h / 2)};

    SDL_RenderCopyEx(
                        mRenderer,
                        mTexture,
                        nullptr,
                        &intHitBox,
                        angle,
                        &centered,
                        SDL_FLIP_NONE
                    );
}

SDL_Texture *Texture::GetTexture(void) const
{
    return mTexture;
}
