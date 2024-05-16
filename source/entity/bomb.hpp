#ifndef __BOMB_HPP__
#define __BOMB_HPP__

#include "../game_object.hpp"
#include "../utils/texture.hpp"
#include <vector>

enum enumBombState
{
    bAlive,
    bDetonate,
    bDead
};

struct ExplosionTile
{
    Texture mTexture;
    const SDL_FRect mHitBox;
    const double mTextureRotationAngle;
};

class Bomb : public GameObject
{
    private:

        int mLifeTimeMSec;

        int mActivationTime;

        float mSpriteUpdateTime;

        enumBombState mBombState;

        SDL_Point mPosOnMap;

        std::vector<ExplosionTile> mExplosionMap;
        
        bool hasBombLeft = false;

    public:

        Bomb(const SDL_FRect& hitBox);

        void Render(void) const override;

        void Update(void) override;

        void Detonate(void);

        void Collision(const SDL_Point& explosionPartPos);


        bool GetHasBombLeft(void);

        void SetBombState(enumBombState newState);

        const SDL_Point& GetMapPos(void);

        enumBombState GetBombState(void);
};

#endif