#ifndef __BOOSTER_HPP__
#define __BOOSTER_HPP__

#include "../game_object.hpp"

enum boostType
{
    Bombs,
    Flames,
    Speed,
    Detonator,
    Bombpass,
    Flamepass 
};

class Booster : public GameObject
{
    private:

        boostType mType;

        int mLifeTimeMSec;

        int mActivationTime;

        SDL_Point mPosOnMap;
     
        float mSpriteUpdateTime;

    public:

        Booster(const SDL_FRect& hitBox);

        void Update(void) override;

        
        
        void PickUp(void);

        void Remove(void);
  
        const boostType& GetType(void);

        const SDL_Point& GetMapPos(void);

        const int GetLifeTimeMSec(void) const;
};

#endif