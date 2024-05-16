#ifndef __MENU_SCENE_HPP__
#define __MENU_SCENE_HPP__

#include "scene.hpp"
#include "button.hpp"

enum MenuSceneButtons
{
    bPlay,
    bAuthor,
    bHelp,
    bExit,
    bBack
};

enum MenuSceneStates
{
    sTitle,
    sAuthor,
    sHelp
};

class MenuScene : public Scene
{
    private:

        static MenuScene mInstance;

        Button** mButtons = nullptr;

        TTF_Font* titleFont = nullptr;

        MenuSceneStates state;

    public: 

        void Init(void);

        void CleanUp(void);

        void Render(void) override;

        void Logic(void) override;

        void Event(SDL_Event* userEvent) override;
    
        static MenuScene* GetInstance(void);
};


#endif
