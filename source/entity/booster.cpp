#include "booster.hpp"
#include "../consts.hpp"
#include "../utils/timer.hpp"
#include "../character.hpp"
#include "../character.hpp"

/*
struct Stats
{
    int numBombs = 2;
    int flamesLen = 1;
    int speed = 50;
    bool detonator = false;
    bool bombPass = false;
    bool flamePass = false;
};
*/

Booster::Booster(const SDL_FRect& hitBox)
{
    auto& stats = Character::GetInstance()->GetStats();
    mType = static_cast<boostType>(rand() % 6);

    if(mType == Detonator && stats.detonator)
        needDelete = true;
    
    else if(mType == Bombpass && stats.bombPass)
        needDelete = true;

    else if(mType == Flamepass && stats.flamePass)
        needDelete = true;
    
    else if(mType == Speed && stats.speed > 50)
        needDelete = true;

    else if(mType == Bombs && stats.numBombs > 4)
        needDelete = true;
    
    else if(mType == Flames && stats.flamesLen > 4)
        needDelete = true;


    mHitBox = hitBox;    
    mSpriteBox = {16*static_cast<int>(mType), 0, 16, 16};
    
    mPosOnMap = { static_cast<int>(hitBox.x / Consts::TILE_WIDTH), static_cast<int>(hitBox.y / Consts::TILE_HEIGHT) };
    mTextureKey = "boosts";

    mLifeTimeMSec = 0;
    mSpriteUpdateTime = 0;
    mActivationTime = Timer::GetTicks();
}


void Booster::Update(void)
{
    float dt = Timer::GetDt();
    mLifeTimeMSec = Timer::GetTicks() - mActivationTime;

    if(mLifeTimeMSec > Consts::BOOSTER_MSEC_LIFE_TIME / 4 * 3)
    {
        mSpriteUpdateTime += dt; 

        if(mSpriteUpdateTime > 0.4)
        {
            mSpriteBox.y = (mSpriteBox.y - mSpriteBox.h) % 32;
            mSpriteUpdateTime = 0;
        }
        if(mLifeTimeMSec > Consts::BOOSTER_MSEC_LIFE_TIME)
        {
            needDelete = true; 
        }    
    }
}


void Booster::PickUp(void)
{
    Stats& stats = Character::GetInstance()->GetStats();
    switch(mType)
    {
        case Bombs:
            stats.numBombs += 1;
            break;
        
        case boostType::Flames:
            stats.flamesLen += 1;
            break;
        
        case boostType::Speed:
            stats.speed += 10;
            break;
                
        case boostType::Detonator:
            stats.detonator = true;
            break;
        
        case boostType::Bombpass:
            stats.bombPass = true;
        
        case boostType::Flamepass:
            stats.flamePass = true;
    }
    needDelete = true;
}

void Booster::Remove(void){
    needDelete = true;
}

const SDL_Point &Booster::GetMapPos(void){
    return mPosOnMap;
}

const int Booster::GetLifeTimeMSec(void) const{
    return mLifeTimeMSec;
}

const boostType &Booster::GetType(void){
    return mType;
}