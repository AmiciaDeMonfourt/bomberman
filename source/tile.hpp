#ifndef __TILE_HPP__
#define __TILE_HPP__

#include "common.hpp"
#include "game_object.hpp"

enum enumTileType
{
    floor,
    brick,
    wall,
    bomboOnTail
};

class Tile : public GameObject
{
    private:

        enumTileType mTileType;
    
    public:

        Tile(enumTileType type, const SDL_FRect& hitBox);

        enumTileType GetTileType(void);

        void SetTileType(enumTileType type);
};

#endif