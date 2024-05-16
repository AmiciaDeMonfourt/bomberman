#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include "..\SDL\include\SDL2\SDL.h"
#include "..\SDL\include\SDL2\SDL_image.h"
#include "..\SDL\include\SDL2\SDL_timer.h"
#include "..\SDL\include\SDL2\SDL_mixer.h"
#include "..\SDL\include\SDL2\SDL_ttf.h" 

#include <exception>
#include <iostream>

void RenderText(const std::string& text, TTF_Font* font, const SDL_Point& textPos, const SDL_Color& textColor);

void RenderText(const std::string& text, const SDL_Point& textPos, const SDL_Color& textColor);

#endif