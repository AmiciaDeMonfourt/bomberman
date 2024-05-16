#include "game_over_scene.hpp"
#include "menu_scene.hpp"
#include "game_scene.hpp"
#include "../utils/texture_map.hpp"
#include "../character.hpp"
#include "../game.hpp"
#include "../map.hpp"

void GameOverScene::Init(void)
{
    const int numButtons = 2;

    mButtons = new Button*[numButtons];

    SDL_FRect bBox = {540, 350, 200, 40};
    mButtons[restart] = new Button("Играть заново", bBox, Consts::WHITE, Consts::GREEN);
    
    bBox = {540, 450, 200, 40};
    mButtons[goMenu] = new Button("Выйти в меню", bBox, Consts::WHITE, Consts::GREEN);

    gameOverFont = TTF_OpenFont("font/glastown.ttf", 128);
}

void GameOverScene::CleanUp(void)
{
    std::cout << "MainScene::CleanUp - begin" << std::endl;
    if(mButtons != nullptr)
    {
        for(int i = 0; i < sizeof(mButtons) / sizeof(mButtons[0]); i++)
             delete mButtons[i];
        delete mButtons;
    }
    TTF_CloseFont(gameOverFont);
    std::cout << "MainScene::CleanUp - end" << std::endl;
}

void GameOverScene::Render(void)
{
    Map::GetInstance()->Render();
    Character::GetInstance()->Render();

    SDL_FRect hitBox = {0, 0, Consts::WINDOW_WIDTH, Consts::WINDOW_HEIGHT};
    TextureMap::Get("vignette")->Render(hitBox);

    
    if(isWin)
    {
        SDL_Point textPos = {500, 200};
        RenderText("Победа!", gameOverFont, textPos, Consts::GREEN);
    }
    else
    {
        SDL_Point textPos = {440, 200};
        RenderText("Поражение!",gameOverFont, textPos, Consts::RED);
    }


    mButtons[restart]->Render();
    mButtons[goMenu]->Render();
}

void GameOverScene::Logic(void)
{
    if(mButtons[restart]->GetIsClick()) 
    {
        Map::GetInstance()->CleanUp();
        mButtons[restart]->Reset();

        Character::GetInstance()->Init();
        Map::GetInstance()->Init();
        
        Game::GetInstance()->SetActiveScene(static_cast<Scene*>(GameScene::GetInstance()));
    }
    if(mButtons[goMenu]->GetIsClick())
    {
        Map::GetInstance()->CleanUp();
        mButtons[goMenu]->Reset();
        
        Game::GetInstance()->SetActiveScene(static_cast<Scene*>(MenuScene::GetInstance()));
    }   
}

void GameOverScene::Event(SDL_Event* userEvent)
{  
    mButtons[restart]->UserInput(userEvent);
    mButtons[goMenu]->UserInput(userEvent);
}

GameOverScene* GameOverScene::GetInstance(void)
{
    return &mInstance;
}

GameOverScene GameOverScene::mInstance;