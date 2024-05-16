#ifndef __BLACK_HOLE_HPP__
#define __BLACK_HOLE_HPP__

#include "enemy.hpp"

class BlackHole : public Enemy
{
    private:

        void Collision(float xOffset, float yOffset) override;

        int frame = 0;

    public:

        BlackHole(const SDL_Point& mapPos);

        void Update(void) override;

        void UpdatePathNotAimed(void) override;

};

#endif