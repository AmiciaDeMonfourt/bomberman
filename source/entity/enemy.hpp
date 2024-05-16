#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "../game_object.hpp"
#include "../consts.hpp"
#include <deque>


class Enemy : public GameObject
{
    protected:

        std::deque<SDL_Point> path;

        SDL_Point mTarget;

        SDL_Point mStartPos;

        SDL_FPoint mVelocity;

        SDL_Rect mMapPos;

        Direction hDir;

        Direction vDir;

        int mWidth = Consts::CHARACTER_GAME_WIDTH;

        int mHeight = Consts::CHARACTER_GAME_HEIGHT;

        float mTimeSpriteUpdate;

        bool mIsAimedPLayer;

        void UpdatePathOrthogonal(void);

        virtual void UpdatePathNotAimed(void);

        virtual void Collision(float xOffset, float yOffset);
    
    public:

        virtual void Update(void) override;
        
        Enemy(const SDL_Point& mapPos);
        virtual ~Enemy() {}

        void Render(void) const override;

        void Kill(void);        

        const SDL_Rect& GetMapPos(void) const;
};

#endif