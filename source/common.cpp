#include "common.hpp"
#include "game.hpp"

void RenderText(const std::string &text, TTF_Font* textFont, const SDL_Point &textPos, const SDL_Color& textColor)
{
    SDL_Renderer* renderer = Game::GetInstance()->GetRenderer();

    if(!textFont) {
        throw std::invalid_argument("RenderText: font == nullptr");
    }
    
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(textFont, text.c_str(), textColor);
    if(!textSurface) {
        throw std::runtime_error(std::string("RenderText: textSurface == nullptr: ") + SDL_GetError());
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(!textTexture) {
        throw std::runtime_error(std::string("RenderText: textTexture == nullptr: ") + SDL_GetError());
    }

    SDL_Rect textBox = {textPos.x, textPos.y, textSurface->w, textSurface->h};
    SDL_RenderCopy(
                        renderer, 
                        textTexture, 
                        nullptr, 
                        &textBox
                  );

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void RenderText(const std::string &text, const SDL_Point &textPos, const SDL_Color& textColor)
{
    TTF_Font* textFont = Game::GetInstance()->GetScreenTextFont();
    SDL_Renderer* renderer = Game::GetInstance()->GetRenderer();

    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(textFont, text.c_str(), textColor);
    if(!textSurface) {
        throw std::runtime_error(std::string("RenderText: textSurface == nullptr: ") + SDL_GetError());
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(!textTexture) {
        throw std::runtime_error(std::string("RenderText: textTexture == nullptr: ") + SDL_GetError());
    }

    SDL_Rect textBox = {textPos.x, textPos.y, textSurface->w, textSurface->h};
    SDL_RenderCopy(
                        renderer, 
                        textTexture, 
                        nullptr, 
                        &textBox
                  );

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

