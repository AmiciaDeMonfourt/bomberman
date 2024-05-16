#include "map.hpp"
#include "consts.hpp"
#include "entity/black_hole.hpp"


void Map::Init(void)
{
    mMapLayot = std::vector<std::vector<int>>(Consts::GAME_MAP_NUM_TILES_Y, std::vector<int>(Consts::GAME_MAP_NUM_TILES_X));


    //выставляем стенки по границам
    for(int y = 0; y < mMapLayot.size(); y++)
        for(int x : {0, Consts::GAME_MAP_NUM_TILES_X-1})
            mMapLayot[y][x] = 2;
    
    
    for(int x = 0; x < Consts::GAME_MAP_NUM_TILES_X; x++)
        for(int y : {0, Consts::GAME_MAP_NUM_TILES_Y-1})
            mMapLayot[y][x] = 2;


    for(int y = 2; y < mMapLayot.size(); y+=2)
        for(int x = 2; x < mMapLayot[0].size(); x+=2)
            mMapLayot[y][x] = 2;


    for(int y = 0; y < mMapLayot.size(); y++){
        for(int x = 0; x < mMapLayot[0].size(); x++){
            if(mMapLayot[y][x] != 2)
            {
                int distrib = rand() % 100;          
                if(distrib < 54){
                    mMapLayot[y][x] = 1;
                }
            }
        }
    }


    int dx[] = {-1, 1, 0,  0};
    int dy[] = { 0, 0, 1, -1};


    for(int gen = 0; gen < 10; gen++)
    {
        auto map_copy = mMapLayot;
        for(int y = 1; y < mMapLayot.size()-1; y++)
        {
            for(int x = 1; x < mMapLayot[0].size()-1; x++)
            {
                int num_neighbours = 0;
                for(int i = 0; i < 4; i++)
                {
                    if(mMapLayot[y+dy[i]][x+dx[i]] != 0)

                        num_neighbours++;
                }

                if(mMapLayot[y][x] == 1 && num_neighbours == 0)
                    map_copy[y][x] = 0;

                else if(mMapLayot[y][x] == 0)
                {
                    if(num_neighbours == 4)
                        map_copy[y][x] = 1;
                }
            }
        }
        mMapLayot = map_copy;
    }


    mMapLayot[1][1] = 0;
    mMapLayot[1][2] = 0;
    mMapLayot[2][1] = 0;
    mMapLayot[1][3] = 1;
    mMapLayot[3][1] = 1;


    for(int y = 0; y < Consts::GAME_MAP_NUM_TILES_Y; y++) 
    {
        std::vector<Tile*> tileLine;
        for(int x = 0; x < Consts::GAME_MAP_NUM_TILES_X; x++) 
        {
            SDL_FRect tileRect = {
                                    (float) Consts::TILE_WIDTH  * x,
                                    (float) Consts::TILE_HEIGHT * y,
                                    (float) Consts::TILE_WIDTH,
                                    (float) Consts::TILE_HEIGHT
                                };

            Tile* newTile = nullptr;
            Enemy* enemy = nullptr;
            switch(mMapLayot[y][x])
            {
                case 0: 
                    if(y > 3 && x > 3 && (rand() % 100 < 20))
                    {
                        SDL_Point startPos = {x,y};
                        enemy = new Enemy(startPos);
                        mEnemyMap.push_back(enemy);
                    }
                    newTile = new Tile(enumTileType::floor, tileRect);
                    break;

                case 1: 
                    newTile = new Tile(enumTileType::brick, tileRect);
                    break;

                case 2: 
                    newTile = new Tile(enumTileType::wall, tileRect);
                    break;
            }

            tileLine.push_back(newTile);
        }
        mGameMap.push_back(tileLine);
    }

    SDL_Point point = {Consts::GAME_MAP_NUM_TILES_X/2-1, Consts::GAME_MAP_NUM_TILES_Y/2-1};
    Enemy* black_hole = new BlackHole(point);
    mEnemyMap.push_back(black_hole);
}


void Map::CleanUp(void)
{
    for(int y = 0; y < mGameMap.size(); y++) {
        for(int x = 0; x < mGameMap[y].size(); x++) {
            delete mGameMap[y][x];
        }
        mGameMap[y].clear();
    }
    mGameMap.clear();

    
    for(int x = 0; x < mBompMap.size(); x++)
        delete mBompMap[x];     
    mBompMap.clear();
    

    for(int x = 0; x < mBoostMap.size(); x++)
        delete mBoostMap[x];
    mBoostMap.clear();


    for(int x = 0; x < mEnemyMap.size(); x++)
        delete mEnemyMap[x];
    mEnemyMap.clear();
}


void Map::Render(void) const
{
    for(int y = 0; y < mGameMap.size(); y++)
        for(int x = 0; x < mGameMap[y].size(); x++)
            mGameMap[y][x]->Render();

    for(int i = 0; i < mBoostMap.size(); i++)
        mBoostMap[i]->Render();

    for(int i = 0; i < mBompMap.size(); i++)
        mBompMap[i]->Render();

    for(int i = 0; i < mEnemyMap.size(); i++)
        mEnemyMap[i]->Render();
}

void Map::Update(void)
{
    for(auto bomb = mBompMap.begin(); bomb < mBompMap.end();)
    {
        (*bomb)->Update();
        if((*bomb)->GetNeedDelete() == true)
        {
            delete *bomb;
            bomb = mBompMap.erase(bomb);
        }
        else
        {
            ++bomb;
        }
    }

    for(auto boost = mBoostMap.begin(); boost < mBoostMap.end();)
    {
        (*boost)->Update();
        if((*boost)->GetNeedDelete() == true)
        {
            delete *boost;
            boost = mBoostMap.erase(boost);
        }
        else
        {
            ++boost;
        }
    }

    for(auto enemy = mEnemyMap.begin(); enemy < mEnemyMap.end();)
    {
        (*enemy)->Update();
        if((*enemy)->GetNeedDelete() == true)
        {
            delete *enemy;
            enemy = mEnemyMap.erase(enemy);
        }
        else
        {
            ++enemy;
        }
    }
}


std::vector<std::vector<Tile*>>& Map::GetMap(void){
    return mGameMap;
}

std::vector<std::vector<int>> &Map::GetMapLayot(void){
    return mMapLayot;
}

std::vector<Bomb*>& Map::GetBombMap(void){
    return mBompMap;
}

std::vector<Enemy *> &Map::GetEnemyMap(void){
    return mEnemyMap;
}

std::vector<Booster *> &Map::GetBoostMap(void){
    return mBoostMap;
}

void Map::AddBooster(Booster *newBoost){
    mBoostMap.push_back(newBoost);
}

void Map::AddBomb(Bomb *newBomb){
    mBompMap.push_back(newBomb);
}

Map *Map::GetInstance(void){
    return &mInstance; 
}

Map Map::mInstance;