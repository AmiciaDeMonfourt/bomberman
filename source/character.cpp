#include "character.hpp"
#include "utils/timer.hpp"
#include "entity/bomb.hpp"
#include "consts.hpp"
#include "map.hpp"


void Character::Init(void)
{
    isAlive = true;
    mHitBox = 
    {
        Consts::TILE_WIDTH,
        Consts::TILE_HEIGHT, 
        Consts::CHARACTER_GAME_WIDTH, 
        Consts::CHARACTER_GAME_HEIGHT
    };
    
    mSpriteBox = 
    {
        0, 
        0, 
        Consts::CHARACTER_SPRITE_WIDTH, 
        Consts::CHARACTER_SPRITE_HEIGHT
    };
    mTextureKey = "earth";
    mVelocity = {0, 0}; 
}


void Character::UserInput(const SDL_Event *event)
{
    if(event->type == SDL_KEYDOWN && event->key.repeat == 0)
    switch(event->key.keysym.scancode)
    {
        case SDL_SCANCODE_UP:            
            mVelocity.y = -mStats.speed;
            break;

        case SDL_SCANCODE_DOWN:            
            mVelocity.y = mStats.speed;
            break;

        case SDL_SCANCODE_RIGHT:            
            mVelocity.x = mStats.speed;
            break;

        case SDL_SCANCODE_LEFT:
            mVelocity.x = -mStats.speed;
            break;
        
        case SDL_SCANCODE_SPACE:
            if(Map::GetInstance()->GetBombMap().size() < mStats.numBombs)
            {      
                auto& map = Map::GetInstance()->GetMap();

                if(map[mMapPos.y][mMapPos.x]->GetTileType() == enumTileType::floor){
                    Bomb* newBomb = new Bomb(mHitBox);
                    Map::GetInstance()->AddBomb(newBomb);
                }
            }
            break;
        
        case SDL_SCANCODE_B:
            if(mStats.detonator == true)
            {
                auto& bombMap = Map::GetInstance()->GetBombMap();
                for(int i = 0; i < bombMap.size(); i++)
                    if(bombMap[i]->GetBombState() == enumBombState::bAlive)
                        bombMap[i]->Detonate();
            }
            break;    
    }
    else if(event->type == SDL_KEYUP)
    {
        switch (event->key.keysym.scancode)
        {
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_UP:
                mVelocity.y = 0;
                break;

            case SDL_SCANCODE_RIGHT:
            case SDL_SCANCODE_LEFT:
                mVelocity.x = 0;
                break;
        }
    }
}

void Character::Update(void)
{   
    static float timeSpriteUpdate = 0;
    float dt = Timer::GetDt();

    timeSpriteUpdate += dt; 
    
    if(timeSpriteUpdate > 0.6)
    {
        mSpriteBox.x = (mSpriteBox.x + Consts::CHARACTER_SPRITE_WIDTH) % 5000;
        timeSpriteUpdate = 0;
    }       

    Collision(mVelocity.x * dt, mVelocity.y * dt);
}

void Character::Collision(float xOffset, float yOffset)
{
    // //количество плиток по осям
    //новые координаты игрока
    float newX  = mHitBox.x + xOffset;
    float newY  = mHitBox.y + yOffset;
    float newXW = mHitBox.x + mWidth + xOffset;
    float newYH = mHitBox.y + mHeight + yOffset;

    //ПРОВЕРКА ЧТО КООРДИНАТЫ НЕ ВЫШЛИ ЗА ЭКРАННЫЕ
    if(newX < 0) newX = 0;
    if(newY < 0) newY = 0;
    if(newXW > Consts::WINDOW_WIDTH ) newX = mHitBox.x;
    if(newYH > Consts::WINDOW_HEIGHT) newY = mHitBox.y;


    SDL_Rect pos = 
    {
        static_cast<int>(newX  / Consts::TILE_WIDTH ),
        static_cast<int>(newY  / Consts::TILE_HEIGHT),
        static_cast<int>(newXW / Consts::TILE_WIDTH ),
        static_cast<int>(newYH / Consts::TILE_HEIGHT)
    };
    
    
    // ОПРЕДЕЛЯЕМ НАПРАВЛЕНИЕ ДВИЖЕНИЕ ОТНОСИТЕЛЬНО СМЕЩЕНИЯ ПО ОСЯМ
    Direction vDir = None;
    Direction hDir = None;


    if(xOffset == 0)     hDir = None;
    else if(xOffset < 0) hDir = Left;
    else if(xOffset > 0) hDir = Right;

    if(yOffset == 0)     vDir = None;
    else if(yOffset < 0) vDir = Up;
    else if(yOffset > 0) vDir = Down;
    
    
    auto& mapL = Map::GetInstance()->GetMapLayot();
    
    //получаем копию схемы карты
    int topLeft  = mapL[pos.y][pos.x]; //левый верхний
    int topRight = mapL[pos.y][pos.w]; //правый верхний
    int botLeft  = mapL[pos.h][pos.x]; //левый нижний
    int botRight = mapL[pos.h][pos.w]; //правый нижний

    //ПРОВЕРЯЕМ В КАКУЮ ТОЧКУ ПРИДЕТ ИГРОК ЕСЛИ ЕСТЬ БЫЛО ОПРЕДЕЛЕНО СМЕЩЕНИЕ ПО ГОРИЗОНТАЛИ
    switch(hDir)
    {
        //ЕСЛИ ВЕКТОР СМЕЩЕНИЯ ПО ГОРИЗОНТАЛИ НАПРАВЛЕН НАПРАВО
        case Right:
            //ЕСЛИ ПРАВАЯ ВЕРХНЯЯ ИЛИ НИЖНЯЯ ТОЧКИ ИГРОКА СТОКНУЛИСЬ СО СТЕНОЙ 
            if(topRight != 0 || botRight != 0) 
            {
                //ЕСЛИ ВЕРТИКАЛЬНЫЙ ВЕКТОР НУЛЕВОЙ И В ПРАВОЙ ВЕРХНЕЙ ТОЧКЕ НАХОДИТСЯ СВОБОДНОЕ ПРОСТРАНСТВО
                if((vDir == None) && (topRight == 0))
                {
                    //ИГРОК ПО Y СКОЛЬЗИТ ВВЕРХ
                    newY = mHitBox.y - xOffset / 3;
                    newYH = mHitBox.y + mHeight + yOffset/3;
                }
                //ЕСЛИ ВЕРТИКАЛЬНЫЙ ВЕКТОР НУЛЕВОЙ И В ПРАВОЙ НИЖНЕЙ ТОЧКЕ НАХОДИТСЯ СВОБОДНОЕ ПРОСТРАНСТВО
                else if((vDir == None) && (botRight == 0))
                {
                    //ИГРОК СКОЛЬЗИТ ПО Y ВНИЗ
                    newY = mHitBox.y + xOffset / 3;
                    newYH = mHitBox.y + mHeight + yOffset/3;
                }
                //СМЕЩЕНИЕ ПО ВЕРТИКАЛИ 
                newX = mHitBox.x;
                newXW = mHitBox.x + mWidth;
            }
            break;

        case Left:
            if(topLeft != 0 || botLeft != 0) 
            {
                if((vDir == None) && (topLeft == 0))
                {
                    newY = mHitBox.y + xOffset / 3;
                    newYH = mHitBox.y + mHeight + yOffset/3;
                }
                else if((vDir == None) && (botLeft == 0))
                {
                    newY = mHitBox.y - xOffset / 3;
                    newYH = mHitBox.y + mHeight + yOffset/3;
                }
                newX = mHitBox.x;
                newXW = mHitBox.x + mWidth;
            }
            break;        
    }
 
    //ПРОВЕРЯЕМ В КАКУЮ ТОЧКУ ПРИДЕТ ИГРОК ЕСЛИ ЕСТЬ БЫЛО ОПРЕДЕЛЕНО СМЕЩЕНИЕ ПО ВЕРТИКАЛИ
    switch(vDir)
    {
        case Up:
            if(topLeft != 0 || topRight != 0) 
            {
                if((hDir == None) && (topRight == 0))
                {
                    newX = mHitBox.x - yOffset / 3;
                    newXW = mHitBox.x + mWidth + yOffset/2;
                }
                else if((hDir == None) && (topLeft == 0))
                {
                    newX = mHitBox.x + yOffset / 3;
                    newXW = mHitBox.x + mWidth + yOffset/2;
                }
                newY = mHitBox.y;
                newYH = mHitBox.y + mHeight;
            }
            break;

        case Down:
            if(botLeft != 0 || botRight != 0) 
            {
                if((hDir == None) && (botRight == 0))
                {
                    newX = mHitBox.x + yOffset / 3;
                    newXW = mHitBox.x + mWidth + yOffset/2;
                }
                else if((hDir == None) && (botLeft == 0))
                {
                    newX = mHitBox.x - yOffset / 3;
                    newXW = mHitBox.x + mWidth + yOffset/2;
                }
                newY = mHitBox.y;
                newYH = mHitBox.y + mHeight;
            }
            break;        
    }

    //ОПРЕДЕЛЯЕМ ПОЗИЦИЮ ИГРОКА ПОСЛЕ АНАЛИЗА СМЕЩЕННОЙ ПОЗИЦИИ
    mHitBox.x = newX;
    mHitBox.y = newY;

    //ПЕРЕВОДИМ ЭКРАННЫЕ КООРДИНАТЫ В КООРДИНАТЫ НА КАРТЕ
    mMapPos = 
    {
        static_cast<int>(newX  / Consts::TILE_WIDTH ),
        static_cast<int>(newY  / Consts::TILE_HEIGHT),
        static_cast<int>(newXW / Consts::TILE_WIDTH ),
        static_cast<int>(newYH / Consts::TILE_HEIGHT)
    };


    //если  
    auto& boostMap = Map::GetInstance()->GetBoostMap();
    for(int i = 0; i < boostMap.size(); i++)
    {
        auto bPos = boostMap[i]->GetMapPos();
        if(mMapPos.x == bPos.x && mMapPos.y == bPos.y || mMapPos.w == bPos.x && mMapPos.h == bPos.y ||
           mMapPos.x == bPos.x && mMapPos.h == bPos.y || mMapPos.w == bPos.x && mMapPos.y == bPos.y)
            boostMap[i]->PickUp();
    }


    if(mStats.bombPass == false)
    {
        auto& bombMap = Map::GetInstance()->GetBombMap();
        for(int i = 0; i < bombMap.size(); i++)
        {
            const SDL_Point& bPos = bombMap[i]->GetMapPos();
            if(mMapPos.x == bPos.x && mMapPos.y == bPos.y || mMapPos.w == bPos.x && mMapPos.h == bPos.y ||
               mMapPos.x == bPos.x && mMapPos.h == bPos.y || mMapPos.w == bPos.x && mMapPos.y == bPos.y)
                {
                    if(bombMap[i]->GetHasBombLeft())
                    {
                        mHitBox.y -= yOffset;
                        mHitBox.x -= xOffset;
                    }
                }
        }
    }
}

const SDL_Rect& Character::GetMapPos(void) const{
    return mMapPos;
}

Stats &Character::GetStats(void){
    return mStats;
}

Character *Character::GetInstance(void){
    return &mInstance;
}

Character Character::mInstance;