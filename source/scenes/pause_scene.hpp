#ifndef __PAUSE_SCENE_HPP__
#define __PAUSE_SCENE_HPP__

#include "scene.hpp"
#include "button.hpp"

enum PauseSceneButtons
{
    resume,
    menu
};

class PauseScene : public Scene
{
    private:

        static PauseScene mInstance;

        Button** mButtons = nullptr;
        
        TTF_Font* pauseFont = nullptr;

    public: 

        void Init(void);

        void CleanUp(void);

        void Render(void) override;

        void Logic(void) override;

        void Event(SDL_Event* userEvent) override;
    
        static PauseScene* GetInstance(void);
};

#endif