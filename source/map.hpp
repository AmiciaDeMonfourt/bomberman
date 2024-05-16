#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "common.hpp"
#include "game_object.hpp"
#include "entity/bomb.hpp"
#include "entity/booster.hpp"
#include "entity/enemy.hpp"
#include "tile.hpp"
#include <vector>

class Map : public GameObject
{
    private:

        static Map mInstance;

        std::vector<std::vector<Tile*>> mGameMap;

        std::vector<std::vector<int>> mMapLayot;

        std::vector<Booster*> mBoostMap;

        std::vector<Enemy*> mEnemyMap;
   
        std::vector<Bomb*> mBompMap;

    public:

        void Init(void);

        void CleanUp(void);

        void Render(void) const override;

        void Update(void) override;


        std::vector<std::vector<Tile*>>& GetMap(void);

        std::vector<std::vector<int>>& GetMapLayot(void);
        
        std::vector<Booster*>& GetBoostMap(void);
        
        std::vector<Bomb*>& GetBombMap(void);

        std::vector<Enemy*>& GetEnemyMap(void);


        void AddBooster(Booster* newBoost);

        void AddBomb(Bomb* newBomb);


        static Map* GetInstance(void);
};

#endif