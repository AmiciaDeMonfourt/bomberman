#ifndef __GAME_OVER_SCENE_HPP__
#define __GAME_OVER_SCENE_HPP__

#include "scene.hpp"
#include "button.hpp"


enum GameSceneButtons
{
    goMenu,
    restart
};


class GameOverScene : public Scene
{
    private:

        static GameOverScene mInstance;

        Button** mButtons = nullptr;

        TTF_Font* gameOverFont = nullptr;

    public: 

        bool isWin = false;

        void Init(void);

        void CleanUp(void);

        void Render(void) override;

        void Logic(void) override;

        void Event(SDL_Event* userEvent) override;
    
        static GameOverScene* GetInstance(void);
};


#endif
