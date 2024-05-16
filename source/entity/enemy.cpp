#include "enemy.hpp"
#include "../utils/timer.hpp"
#include "../tile.hpp"
#include "../map.hpp"
#include "../character.hpp"
#include "../utils/texture_map.hpp"


Enemy::Enemy(const SDL_Point& mapPos)
{
    int sprite = rand() % 6;
    mTextureKey = "enemy" + std::to_string(sprite + 1);

    mHitBox = 
    {
        static_cast<float>(mapPos.x * Consts::TILE_WIDTH+2), 
        static_cast<float>(mapPos.y * Consts::TILE_HEIGHT+2), 
        Consts::CHARACTER_GAME_WIDTH, 
        Consts::CHARACTER_GAME_HEIGHT
    };
    
    mTimeSpriteUpdate = 0;
    mIsAimedPLayer = false;

    mVelocity = {30, 30};
    mSpriteBox = {0, 0, Consts::CHARACTER_SPRITE_WIDTH, Consts::CHARACTER_SPRITE_HEIGHT};

    mStartPos = {mapPos.x, mapPos.y};
    mMapPos = {mapPos.x, mapPos.y, mMapPos.x * Consts::TILE_WIDTH, mapPos.y * Consts::TILE_HEIGHT};
    
    UpdatePathNotAimed();
}

void Enemy::Update(void)
{
    float dt = Timer::GetDt();

    mTimeSpriteUpdate += dt; 
    if(mTimeSpriteUpdate > 0.6)
    { 
        mSpriteBox.x = (mSpriteBox.x + Consts::CHARACTER_SPRITE_WIDTH) % 5000;
        mTimeSpriteUpdate = 0;
        path.clear();
        
        if(!mIsAimedPLayer) 
            UpdatePathNotAimed();
        else
            UpdatePathOrthogonal();
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

void Enemy::Collision(float xOffset, float yOffset)
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
            if(topRight != 0 || botRight != 0) 
            {
                if((vDir == Down) && (botLeft != 0))
                {
                    newY = mHitBox.y;
                    newYH = mHitBox.y + mHeight;
                }
                else if((vDir == Up) && (topLeft != 0))
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
            if((topLeft != 0) || (botLeft != 0))  
            {
                if((vDir == Down) && (botRight != 0))
                {
                    newY = mHitBox.y;
                    newYH = mHitBox.y + mHeight;
                }
                else if((vDir == Up) && (topRight != 0))
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
            if(topLeft != 0 || topRight != 0) 
            {
                if((hDir == Left) && (botLeft != 0))
                {
                    newX = mHitBox.x;
                    newXW = mHitBox.x + mWidth;
                }
                else if((hDir == Right) && (botRight != 0))
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
            if(botLeft != 0 || botRight != 0) 
            {
                if((hDir == Left) && (topLeft != 0))
                {
                    newX = mHitBox.x;
                    newXW = mHitBox.x + mWidth;
                }
                else if((hDir == Right) && (topRight != 0))
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

    pos = Character::GetInstance()->GetMapPos();
    if(mMapPos.x == pos.x && mMapPos.y == pos.y || 
            mMapPos.w == pos.x && mMapPos.h == pos.y || 
                mMapPos.x == pos.x && mMapPos.h == pos.y || 
                    mMapPos.w == pos.x && mMapPos.y == pos.y){
        Character::GetInstance()->isAlive = false;
    }
}


void Enemy::UpdatePathNotAimed(void)
{
    int dx[] = { 0, 1, 0, -1};
    int dy[] = {-1, 0, 1,  0};
    // srand(Timer::GetTicks());
    int changeDir = rand() % 100;
    auto& mapL = Map::GetInstance()->GetMapLayot();

    if(changeDir < 50)
    {
        SDL_Point new_aim;

        int dirDistrib = rand() % 4;

        new_aim = {mMapPos.x + dx[dirDistrib], mMapPos.y + dy[dirDistrib]};

        if(mapL[new_aim.y][new_aim.x] == 0){
            mTarget = new_aim;
        }
    }
}


void Enemy::UpdatePathOrthogonal(void)
{
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    int dlen = 4;
    
 
    auto mapLayotCopy = Map::GetInstance()->GetMapLayot();
    auto& map = Map::GetInstance()->GetMap();

    int numY = mapLayotCopy.size();
    int numX = mapLayotCopy[0].size();

    for(int i = 0; i < numY; i++) 
    {
        for(int j = 0; j < numX; j++) 
        {
            if(mapLayotCopy[i][j] == 0) 
                mapLayotCopy[i][j] = -1;
            else if(mapLayotCopy[i][j] == 1) 
                mapLayotCopy[i][j] = -2;
            else if(mapLayotCopy[i][j] == 2) 
                mapLayotCopy[i][j] = -2;

            if(map[i][j]->GetTileType() == bomboOnTail)
                mapLayotCopy[i][j] = -2;
        }
    }

    mMapPos = {
                static_cast<int>( mHitBox.x / Consts::TILE_WIDTH ), 
                static_cast<int>( mHitBox.y / Consts::TILE_HEIGHT),
                static_cast<int>( (mHitBox.x + mHitBox.w) / Consts::TILE_WIDTH ),
                static_cast<int>( (mHitBox.y + mHitBox.h) / Consts::TILE_HEIGHT)
              };


    SDL_Point startPos;
    switch(vDir)
    {
        case Up:
            switch(hDir)
            {
               case Right:
                    startPos = {mMapPos.x, mMapPos.h};  
                    break;

                case Left:
                    startPos = {mMapPos.w, mMapPos.h};
                    break;
                
                case None:
                    startPos = {mMapPos.x, mMapPos.h};
                    break;
            }
            break;

        case Down:
            switch(hDir)
            {
               case Right:
                    startPos = {mMapPos.x, mMapPos.y};  
                    break;

                case Left:
                    startPos = {mMapPos.w, mMapPos.y};  
                    break;
                
                case None:
                    startPos = {mMapPos.x, mMapPos.y};  
                    break;
            }
            break;
        
        case None:
            switch(hDir)
            {
               case Right:
                    startPos = {mMapPos.x, mMapPos.y};  
                    break;

                case Left:
                    startPos = {mMapPos.w, mMapPos.y};  
                    break;

                case None:
                    startPos = {mMapPos.x, mMapPos.y};  
                    break;
            }
    }

    SDL_Point finalPos = {Character::GetInstance()->GetMapPos().x, Character::GetInstance()->GetMapPos().y};

    if(mapLayotCopy[finalPos.y][finalPos.x] == -2){
        mIsAimedPLayer = false;
        return;
    }

    path.push_back(startPos);

    mapLayotCopy[startPos.y][startPos.x] = 0;

    while(!path.empty())
    {
        SDL_Point currPoint = path.front();
        path.pop_front();

        for(int i = 0; i < 4; i++)
        {
            SDL_Point neighbPos = {currPoint.x + dx[i], currPoint.y + dy[i]};

            if(neighbPos.x > 0 && neighbPos.x < numX && neighbPos.y > 0 && neighbPos.y < numY)
            {
                if(mapLayotCopy[neighbPos.y][neighbPos.x] == -1) 
                {
                    mapLayotCopy[neighbPos.y][neighbPos.x] = mapLayotCopy[currPoint.y][currPoint.x] + 1;
                    path.push_back(neighbPos);
                }
            } 
        }
    }

 
    if(mapLayotCopy[finalPos.y][finalPos.x] == -1 || mapLayotCopy[finalPos.y][finalPos.x] > 10){
        path.clear();
        SDL_Point curpos = {mMapPos.x, mMapPos.y};
        path.push_back(curpos);
        // mTarget = curpos;
        mIsAimedPLayer = false;
        return;
    }

    SDL_Point point = finalPos;
    path.push_back(point);

    while(point.x != startPos.x || point.y != startPos.y)
    {
        for(int i = 0; i < dlen; i++)
        {
            SDL_Point neighbPos = {point.x + dx[i], point.y + dy[i]};
            if(neighbPos.x > 0 && neighbPos.x < numX && neighbPos.y > 0 && neighbPos.y < numY)
            {
                if(mapLayotCopy[neighbPos.y][neighbPos.x] == mapLayotCopy[point.y][point.x] - 1)
                {
                    point = neighbPos;
                    path.push_back(point);
                    break;
                }
            }
        }
    }
    
    path.pop_back();
    mTarget = path.back();
    mIsAimedPLayer = false;
}


void Enemy::Render(void) const
{
    GameObject::Render();

    // for(auto i = path.begin(); i != path.end();){
    //     SDL_FRect pos = {(*i).x * Consts::TILE_WIDTH * 1.f, (*i).y * Consts::TILE_WIDTH * 1.f, (float)Consts::CHARACTER_GAME_WIDTH, (float)Consts::CHARACTER_GAME_HEIGHT};
    //     TextureMap::Get("bomb")->Render(pos);
    //     ++i;
    // }
    
    // SDL_FRect pos = {mTarget.x * Consts::TILE_WIDTH * 1.f, mTarget.y * Consts::TILE_WIDTH * 1.f, (float)Consts::CHARACTER_GAME_WIDTH, (float)Consts::CHARACTER_GAME_HEIGHT};
    // TextureMap::Get("bomb")->Render(pos);
}


void Enemy::Kill(void){
    needDelete = true;
}

const SDL_Rect &Enemy::GetMapPos(void) const {
    return mMapPos;
}
