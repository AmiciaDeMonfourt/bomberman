#include "tile.hpp"
#include "consts.hpp"
#include "map.hpp"

Tile::Tile(enumTileType type, const SDL_FRect &hitBox)
{
    mTileType = type;

    if(mTileType == enumTileType::floor)
        mTextureKey = "floor";

    else if(mTileType == enumTileType::brick)
        mTextureKey = "brick";

    else if(mTileType == enumTileType::wall)
        mTextureKey = "wall";

    mHitBox = hitBox;

    mSpriteBox = {0, 0, Consts::TILE_WIDTH, Consts::TILE_HEIGHT};
}


enumTileType Tile::GetTileType(void)
{
    return mTileType;
}

void Tile::SetTileType(enumTileType type)
{
    auto& mapLayot = Map::GetInstance()->GetMapLayot();

    SDL_Point mapPos 
    {
        static_cast<int>(mHitBox.x / Consts::TILE_WIDTH ),
        static_cast<int>(mHitBox.y / Consts::TILE_HEIGHT)
    };

    mTileType = type;

    if(mTileType == floor)
    {
        mTextureKey = "floor";
        mapLayot[mapPos.y][mapPos.x] = 0;
    }

    else if(mTileType == brick)
    {
        mTextureKey = "brick";
        mapLayot[mapPos.y][mapPos.x] = 1;
    }

    else if(mTileType == wall)
    {
        mTextureKey = "wall";
        mapLayot[mapPos.y][mapPos.x] = 2;
    }
    
    else if(mTileType == bomboOnTail)
    {
        mTextureKey = "floor";
        mapLayot[mapPos.y][mapPos.x] = 0;
    }
}
