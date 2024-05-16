#include "pause_scene.hpp"
#include "game_scene.hpp"
#include "menu_scene.hpp"
#include "../utils/texture_map.hpp"
#include "../consts.hpp"
#include "../game.hpp"
#include "../map.hpp"

void PauseScene::Init(void)
{
    const int numButtons = 2;

    mButtons = new Button*[numButtons];

    SDL_FRect bBox = {540, 350, 200, 40};
    mButtons[resume] = new Button("Продолжить", bBox, Consts::WHITE, Consts::GREEN);
    
    bBox = {540, 450, 200, 40};
    mButtons[menu] = new Button("Выйти в меню", bBox, Consts::WHITE, Consts::GREEN);

    pauseFont = TTF_OpenFont("font/glastown.ttf", 90);
}
 
void PauseScene::CleanUp(void)
{
    std::cout << "PauseScene::CleanUp - begin" << std::endl;
    if(mButtons != nullptr)
    {
        for(int i = 0; i < sizeof(mButtons) / sizeof(mButtons[0]); i++)
            delete mButtons[i];
        delete mButtons;
    }
    std::cout << "PauseScene::CleanUp - end" << std::endl;
}


void PauseScene::Render(void)
{
    GameScene::GetInstance()->Render();

    SDL_FRect hitBox {0, 0, Consts::WINDOW_WIDTH, Consts::WINDOW_HEIGHT};
    TextureMap::Get("vignette")->Render(hitBox);

    SDL_Point pos = {550, 190};
    RenderText("Пауза", pauseFont,  pos, Consts::WHITE);

    mButtons[resume]->Render();
    mButtons[menu]->Render();
}


void PauseScene::Event(SDL_Event* userEvent)
{
    mButtons[resume]->UserInput(userEvent);
    mButtons[menu]->UserInput(userEvent);
}

void PauseScene::Logic(void)
{
    if(mButtons[resume]->GetIsClick()) 
    {
        Game::GetInstance()->SetActiveScene(static_cast<Scene*>(GameScene::GetInstance()));
        mButtons[resume]->Reset();
    }
    if(mButtons[menu]->GetIsClick())
    {
        Map::GetInstance()->CleanUp();
        Game::GetInstance()->SetActiveScene(static_cast<Scene*>(MenuScene::GetInstance()));
        mButtons[menu]->Reset();
    }
}


PauseScene *PauseScene::GetInstance(void){
    return &mInstance;
}

PauseScene PauseScene::mInstance;
