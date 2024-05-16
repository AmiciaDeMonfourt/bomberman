#ifndef __GAME_SCENE_HPP__
#define __GAME_SCENE_HPP__

#include "scene.hpp"
#include "button.hpp"

class GameScene : public Scene
{
    private:

        static GameScene mInstance;

    public: 

        void Render(void) override;

        void Logic(void) override;

        void Event(SDL_Event* userEvent) override;
    
        static GameScene* GetInstance(void);
};


#endif
