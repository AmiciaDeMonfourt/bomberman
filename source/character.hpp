#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__


#include "game_object.hpp"
#include "consts.hpp"

struct Stats
{
    int numBombs = 2;
    int flamesLen = 1;
    int speed = 50;
    bool detonator = false;
    bool bombPass = false;
    bool flamePass = false;
};


class Character : public GameObject
{
    private:

        static Character mInstance;

        SDL_FPoint mVelocity;

        SDL_Rect mMapPos;

        Stats mStats;

        int mWidth = Consts::CHARACTER_GAME_WIDTH;

        int mHeight = Consts::CHARACTER_GAME_HEIGHT;

    public:

        bool isAlive;

        void Init(void);

        void Collision(float xOffset, float yOffset);
        
        void UserInput(const SDL_Event* event) override;

        void Update(void) override;

        const SDL_Rect& GetMapPos(void) const;

        Stats& GetStats(void);

        static Character* GetInstance(void);
};


#endif