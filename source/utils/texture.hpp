#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include "..\common.hpp"


class Texture
{
    private:

        SDL_Texture* mTexture;

        SDL_Renderer* mRenderer;

    public:

        Texture(const std::string& fileName, SDL_Renderer* renderer);

        void CleanUp(void);

        SDL_Texture* GetTexture(void) const;

        void Render(const SDL_FRect& hitBox, const SDL_Rect& spriteBox) const;

        void Render(const SDL_FRect& hitBox) const;

        void Render(const SDL_FRect& hitBox, double angle) const; 
};

#endif
