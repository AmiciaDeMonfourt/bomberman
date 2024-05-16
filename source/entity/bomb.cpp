#include "bomb.hpp"
#include "booster.hpp"
#include "../map.hpp"
#include "../consts.hpp"
#include "../utils/timer.hpp"
#include "../character.hpp"
#include "../utils/texture_map.hpp"
#include "../character.hpp"

Bomb::Bomb(const SDL_FRect& hitBox)
{
    mPosOnMap = {static_cast<int>(hitBox.x / Consts::TILE_WIDTH), static_cast<int>(hitBox.y / Consts::TILE_HEIGHT)};

    mHitBox = {(float) mPosOnMap.x * Consts::TILE_WIDTH, (float) mPosOnMap.y * Consts::TILE_HEIGHT, Consts::TILE_WIDTH, Consts::TILE_HEIGHT};

    mSpriteBox = {0, 0, Consts::BOMB_SPRITE_WIDTH, Consts::BOMB_SPRITE_HEIGHT};

    mTextureKey = "bomb";

    mSpriteUpdateTime = 0;

    mActivationTime = Timer::GetTicks();

    mBombState = enumBombState::bAlive;

    auto& map =  Map::GetInstance()->GetMap();
    map[mPosOnMap.y][mPosOnMap.x]->SetTileType(bomboOnTail);

    needDelete = false;
}

void Bomb::Update(void)
{
    float dt = Timer::GetDt();
    
    switch (mBombState)
    {
        case enumBombState::bAlive:
            if(hasBombLeft == false)
            {
                auto& pos = Character::GetInstance()->GetMapPos();
                if((pos.x != mPosOnMap.x && pos.w != mPosOnMap.x) || (pos.y != mPosOnMap.y && pos.h != mPosOnMap.y))
                    hasBombLeft = true;
            }
            mLifeTimeMSec = Timer::GetTicks() - mActivationTime;
            mSpriteUpdateTime += dt; 
            if(mSpriteUpdateTime > 0.6)
            {
                mSpriteBox.x = (mSpriteBox.x + Consts::BOMB_SPRITE_WIDTH) % 48;
                mSpriteUpdateTime = 0;
            }
            if(mLifeTimeMSec > Consts::BOMB_MSEC_LIFE_TIME)
            {
                Detonate();
            }
            break;
    
        case enumBombState::bDetonate:
        {
            for(int i = 0; i < mExplosionMap.size(); i++) 
                SDL_SetTextureAlphaMod(mExplosionMap[i].mTexture.GetTexture(), mSpriteUpdateTime); 
            
            mSpriteUpdateTime -= 90 * dt;
            
            if(mSpriteUpdateTime < 0)
                mBombState = bDead;
            break;
        }
        
        case enumBombState::bDead:
            auto& map =  Map::GetInstance()->GetMap();
            map[mPosOnMap.y][mPosOnMap.x]->SetTileType(floor);
            needDelete = true;
            break;
    }

}


void Bomb::Detonate(void)
{    
    mBombState = enumBombState::bDetonate;   
    mSpriteUpdateTime = 255;

    auto& gameMap = Map::GetInstance()->GetMap();
    int explosionLength = Character::GetInstance()->GetStats().flamesLen;
    
    ExplosionTile explosTile = {*TextureMap::Get("explosionCenter"), mHitBox, 0};
    mExplosionMap.push_back(explosTile);
    const SDL_Point centerExplosPos = { 
        static_cast<int>(mHitBox.x / Consts::TILE_WIDTH ), 
        static_cast<int>(mHitBox.y / Consts::TILE_HEIGHT), 
    };
    Collision(centerExplosPos);


    for(int direction = Direction::Up; direction <= Direction::Left; direction++)
    {
        Texture currentTexture = *TextureMap::Get("explosionSide");
        int dx = 0, dy = 0;
        double angle = 0;

        switch (direction)
        {
            case Direction::Up:
                dy = -1;
                angle = 0;
                break;
            case Direction::Right:
                dx = 1;
                angle = 90;
                break;
            case Direction::Down:
                dy = 1;
                angle = 180;
                break;
            case Direction::Left:
                dx = -1;
                angle = 270;
                break;
        }

        for (int i = 1; i <= explosionLength; i++)
        {
            if(i == explosionLength){
                currentTexture = *TextureMap::Get("explosionEnd");
            }

            const SDL_Point tileUnderExplosionMapPos = {mPosOnMap.x + dx*i, mPosOnMap.y + dy*i};

            Tile* tileUnderExplosion = gameMap[tileUnderExplosionMapPos.y][tileUnderExplosionMapPos.x];
            
            
            if(tileUnderExplosion->GetTileType() == wall) {
                break;
            }

            const SDL_FRect newExplosRect = { 
                                            tileUnderExplosionMapPos.x * Consts::TILE_WIDTH * 1.f, 
                                            tileUnderExplosionMapPos.y * Consts::TILE_HEIGHT * 1.f, 
                                            mHitBox.w, 
                                            mHitBox.h 
                                        };
            
            if(tileUnderExplosion->GetTileType() == brick) 
            {
                i = explosionLength;
                currentTexture= *TextureMap::Get("explosionEnd");
                tileUnderExplosion->SetTileType(floor);   

                if(rand() % 100 < 40)
                {
                    Booster* newBoost = new Booster(newExplosRect);
                    Map::GetInstance()->AddBooster(newBoost);
                }
            }


            ExplosionTile newExplosTile = { currentTexture, newExplosRect, angle };
            Collision(tileUnderExplosionMapPos);
            mExplosionMap.push_back(newExplosTile);
        }
    }

}



void Bomb::Collision(const SDL_Point& tilePos)
{        
    const SDL_Rect& charMapPos = Character::GetInstance()->GetMapPos();
  
  
    auto& bombMap = Map::GetInstance()->GetBombMap();
    for(int i = 0; i < bombMap.size(); i++) {
        const SDL_Point& bPos = bombMap[i]->mPosOnMap;
        if((bPos.x == tilePos.x) && (bPos.y == tilePos.y) && (bombMap[i]->GetBombState() == bAlive))
            bombMap[i]->Detonate();
    }


    auto& enemyMap = Map::GetInstance()->GetEnemyMap();
    for(int i = 0; i < enemyMap.size(); i++) {
        const SDL_Rect& ePos = enemyMap[i]->GetMapPos();
        if((ePos.x == tilePos.x) && (ePos.y == tilePos.y)) {
            enemyMap[i]->Kill();
        }
    }

    auto& boostMap = Map::GetInstance()->GetBoostMap();
    for(int i = 0; i < boostMap.size(); i++) {
        const SDL_Point& bPos = boostMap[i]->GetMapPos();
        if((bPos.x == tilePos.x) && (bPos.y == tilePos.y)) {
            if(boostMap[i]->GetLifeTimeMSec() > 0){
                boostMap[i]->Remove();
            }
        }
    }

    if((tilePos.x == charMapPos.x || tilePos.x == charMapPos.w) && (tilePos.y == charMapPos.y || tilePos.y == charMapPos.h))
        if(Character::GetInstance()->GetStats().flamePass == false)
            Character::GetInstance()->isAlive = false;
}


void Bomb::Render(void) const
{
    switch(mBombState)
    {
        case enumBombState::bAlive:
            GameObject::Render();
            break;

        case enumBombState::bDetonate:
            for(int i = 0; i < mExplosionMap.size(); i++) 
                mExplosionMap[i].mTexture.Render(mExplosionMap[i].mHitBox, mExplosionMap[i].mTextureRotationAngle);

            break;
    }
}


enumBombState Bomb::GetBombState(void){
    return mBombState;
}

bool Bomb::GetHasBombLeft(void){
    return hasBombLeft;
}

void Bomb::SetBombState(enumBombState newState){
    mBombState = newState;
}


const SDL_Point &Bomb::GetMapPos(void){
    return mPosOnMap;
}
