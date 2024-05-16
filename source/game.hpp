#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "consts.hpp"
#include "common.hpp"

class Scene;


class Game
{
    private:

        static Game mInstance;

        SDL_Window* mWindow;

        SDL_Renderer* mRenderer;

        Scene* mActiveScene;

        TTF_Font* mScreenTextFont;

        bool mRunning = false;

    public:

        ~Game(); 

        void Init(void);

        void Loop(void);
    
        void SetActiveScene(Scene* newScene);

        void SetRunning(bool state);

        SDL_Renderer* GetRenderer(void) const;

        SDL_Window* GetWindow(void) const;

        TTF_Font* GetScreenTextFont(void) const;

        static Game* GetInstance(void);

};

#endif