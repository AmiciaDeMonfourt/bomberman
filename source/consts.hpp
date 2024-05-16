#include "common.hpp"

#ifndef __CONSTS_HPP__
#define __CONSTS_HPP__

namespace Consts
{   
    //the smaller the faster
    constexpr float FACTOR_GAME_SPEED = 300.f;

    constexpr int CHARACTER_SPRITE_WIDTH = 100;

    constexpr int CHARACTER_SPRITE_HEIGHT = 100;

    constexpr float CHARACTER_VELOCITY_Y = 50.f;

    constexpr float CHARACTER_VELOCITY_X = 50.f;


    constexpr int CHARACTER_GAME_WIDTH = 50;

    constexpr int CHARACTER_GAME_HEIGHT = 50;

    constexpr int TILE_WIDTH  = 56;
    
    constexpr int TILE_HEIGHT = 56;


    constexpr int GAME_MAP_NUM_TILES_X = 23;//WINDOW_WIDTH / TILE_WIDTH;

    constexpr int GAME_MAP_NUM_TILES_Y = 15;//WINDOW_HEIGHT / TILE_HEIGHT;


    constexpr int WINDOW_WIDTH  = GAME_MAP_NUM_TILES_X * TILE_WIDTH;//1024
    
    constexpr int WINDOW_HEIGHT = GAME_MAP_NUM_TILES_Y * TILE_HEIGHT;//768;


    constexpr int BOMB_SPRITE_WIDTH  = 16;

    constexpr int BOMB_SPRITE_HEIGHT = 16;

    constexpr float BOMB_MSEC_LIFE_TIME = 4000;

    constexpr float BOOSTER_MSEC_LIFE_TIME = 5000;

    constexpr SDL_Color GREEN   = {21, 140, 90, 255};

    constexpr SDL_Color RED  = {200, 6, 4, 255};

    constexpr SDL_Color WHITE  = {255, 255, 255, 255};
};

#endif