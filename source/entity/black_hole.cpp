#include "black_hole.hpp"
#include "..\utils\timer.hpp"
#include "..\character.hpp"
#include "..\map.hpp"
BlackHole::BlackHole(const SDL_Point& mapPos) : Enemy(mapPos)
{
    mHitBox = 
    {
        static_cast<float>(mapPos.x * Consts::TILE_WIDTH ), 
        static_cast<float>(mapPos.y * Consts::TILE_HEIGHT), 
        50, 
        50
    };

    mTextureKey = "black-hole";

    mWidth = 50;
    mHeight = 50;


    mSpriteBox = {0, 0, 900, 900};
}


void BlackHole::Update(void)
{
    static int frame = 0;
    float multieplie = 1.f;
    float dt = Timer::GetDt();

    mTimeSpriteUpdate += dt; 
    if(mTimeSpriteUpdate > 0.6)
    { 
        if(multieplie > 0)
        {
            if(frame > 20)
            {
                multieplie = -1;
            }
        }
        else
        {
            if(frame < 0)
            {
                multieplie = 1;
            }
        }
    
        frame += multieplie;
        
        mHitBox.h += multieplie;        
        mHitBox.w += multieplie;    

        // mWidth = mHitBox.w;
        // mHeight = mHitBox.h;    
        
        mTimeSpriteUpdate = 0;
        UpdatePathNotAimed();
    }
   
    float dx;
    if(mMapPos.x == mTarget.x && mMapPos.w == mTarget.x)
    {
        hDir = None;
        dx = 0;
    }
    else if(mMapPos.x > mTarget.x || mMapPos.w > mTarget.x)
    {
        hDir = Left;
        dx = -mVelocity.x * dt;
    }
    else if(mMapPos.x < mTarget.x || mMapPos.w < mTarget.x)
    {
        hDir = Right;
        dx = mVelocity.x * dt;
    }

    float dy;
    if(mMapPos.y == mTarget.y && mMapPos.h == mTarget.y)
    {
        vDir = None;
        dy = 0;
    }
    else if(mMapPos.y > mTarget.y || mMapPos.h > mTarget.y)
    {
        vDir = Up;
        dy = -mVelocity.y * dt;
    }
    else if(mMapPos.y < mTarget.y || mMapPos.h < mTarget.y)
    {
        vDir = Down;
        dy = mVelocity.y * dt;
    }

    Collision(dx, dy);
}

void BlackHole::UpdatePathNotAimed(void)
{
    int changeDir = rand() % 100;

    if(changeDir < 50)
    {
        auto& mapL = Map::GetInstance()->GetMapLayot();
        SDL_Point new_aim;
      
        int dx[] = { 0, 1, 0, -1};
        int dy[] = {-1, 0, 1,  0};

        int dirDistrib = rand() % 4;
        
        new_aim = {mMapPos.x + dx[dirDistrib], mMapPos.y + dy[dirDistrib]};

        if(mapL[new_aim.y][new_aim.x] == 0){
            mTarget = new_aim;
        }
    }
}

void BlackHole::Collision(float xOffset, float yOffset)
{
    //новые координаты ботика
    float newX  = mHitBox.x + xOffset;
    float newY  = mHitBox.y + yOffset;
    float newXW = mHitBox.x + mWidth + xOffset;
    float newYH = mHitBox.y + mHeight + yOffset;

    //проверка что ботик не вышел за экран
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
    
    auto& mapL = Map::GetInstance()->GetMapLayot();
    
    //получаем копию схемы карты
    int topLeft  = mapL[pos.y][pos.x]; //левый верхний
    int topRight = mapL[pos.y][pos.w]; //правый верхний
    int botLeft  = mapL[pos.h][pos.x]; //левый нижний
    int botRight = mapL[pos.h][pos.w]; //правый нижний

    switch(hDir)
    {
        case Right:
            if(topRight == 2 || botRight == 2) 
            {
                if((vDir == Down) && (botLeft == 2))
                {
                    newY = mHitBox.y;
                    newYH = mHitBox.y + mHeight;
                }
                else if((vDir == Up) && (topLeft == 2))
                {
                    newY = mHitBox.y;
                    newYH = mHitBox.y + mHeight;
                }
                else
                {
                    newX = mHitBox.x;
                    newXW = mHitBox.x + mWidth;
                }
            }
            break;

        case Left:
            if((topLeft == 2) || (botLeft == 2))  
            {
                if((vDir == Down) && (botRight == 2))
                {
                    newY = mHitBox.y;
                    newYH = mHitBox.y + mHeight;
                }
                else if((vDir == Up) && (topRight == 2))
                {
                    newY = mHitBox.y;
                    newYH = mHitBox.y + mHeight;
                }
                else
                {
                    newX = mHitBox.x;
                    newXW = mHitBox.x + mWidth;
                }
            }
            break; 
    }

    switch(vDir)
    {
        case Up:
            if(topLeft == 2 || topRight == 2) 
            {
                if((hDir == Left) && (botLeft == 2))
                {
                    newX = mHitBox.x;
                    newXW = mHitBox.x + mWidth;
                }
                else if((hDir == Right) && (botRight == 2))
                {
                    newX = mHitBox.x;
                    newXW = mHitBox.x + mWidth;
                }
                else
                {
                    newY = mHitBox.y;
                    newYH = mHitBox.y + mHeight;
                }            
            }
            break;

        case Down:
            if(botLeft == 2 || botRight == 2) 
            {
                if((hDir == Left) && (topLeft == 2))
                {
                    newX = mHitBox.x;
                    newXW = mHitBox.x + mWidth;
                }
                else if((hDir == Right) && (topRight == 2))
                {
                    newX = mHitBox.x;
                    newXW = mHitBox.x + mWidth;
                }
                else
                {
                    newY = mHitBox.y;
                    newYH = mHitBox.y + mHeight;
                }               
            }
            break;        
    }

    mHitBox.x = newX;
    mHitBox.y = newY;

    mMapPos = 
    {
        static_cast<int>(newX  / Consts::TILE_WIDTH ),
        static_cast<int>(newY  / Consts::TILE_HEIGHT),
        static_cast<int>(newXW / Consts::TILE_WIDTH ),
        static_cast<int>(newYH / Consts::TILE_HEIGHT)
    };


    auto& map = Map::GetInstance()->GetMap();

    //убиваем все чего коснулась дыра
    map[mMapPos.y][mMapPos.x]->SetTileType(floor);
    map[mMapPos.h][mMapPos.w]->SetTileType(floor);
    map[mMapPos.y][mMapPos.w]->SetTileType(floor);
    map[mMapPos.h][mMapPos.x]->SetTileType(floor);


    auto& bombs = Map::GetInstance()->GetBombMap();
    for(int i = 0; i < bombs.size(); i++){
        const SDL_Point& bPos = bombs[i]->GetMapPos();
        if(mMapPos.x == bPos.x && mMapPos.y == bPos.y || mMapPos.w == bPos.x && mMapPos.h == bPos.y ||
            mMapPos.x == bPos.x && mMapPos.h == bPos.y || mMapPos.w == bPos.x && mMapPos.y == bPos.y){
                bombs[i]->SetNeedDelete(true);
            }
    }


    auto& enemy = Map::GetInstance()->GetEnemyMap();
    for(int i = 0; i < enemy.size(); i++){
        const SDL_Rect& pos = enemy[i]->GetMapPos();
        if(mMapPos.x == pos.x && mMapPos.y == pos.y || mMapPos.w == pos.x && mMapPos.h == pos.y ||
            mMapPos.x == pos.x && mMapPos.h == pos.y || mMapPos.w == pos.x && mMapPos.y == pos.y){
                if(dynamic_cast<BlackHole*>(enemy[i]) == nullptr)
                    enemy[i]->SetNeedDelete(true);
            }
    }

    auto& boost = Map::GetInstance()->GetBoostMap();
    for(int i = 0; i < boost.size(); i++){
        const SDL_Point& pos = boost[i]->GetMapPos();
        if(mMapPos.x == pos.x && mMapPos.y == pos.y || mMapPos.w == pos.x && mMapPos.h == pos.y ||
            mMapPos.x == pos.x && mMapPos.h == pos.y || mMapPos.w == pos.x && mMapPos.y == pos.y){
                if(dynamic_cast<BlackHole*>(boost[i]) == nullptr)
                    boost[i]->SetNeedDelete(true);
            }
    }

    pos = Character::GetInstance()->GetMapPos();
    if(mMapPos.x == pos.x && mMapPos.y == pos.y || 
            mMapPos.w == pos.x && mMapPos.h == pos.y || 
                mMapPos.x == pos.x && mMapPos.h == pos.y || 
                    mMapPos.w == pos.x && mMapPos.y == pos.y){
        Character::GetInstance()->isAlive = false;
    }
}