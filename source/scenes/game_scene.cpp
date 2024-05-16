#include "game_scene.hpp"
#include "pause_scene.hpp"
#include "game_over_scene.hpp"
#include "../character.hpp"
#include "../game.hpp"
#include "../map.hpp"

void GameScene::Render(void)
{
    Map::GetInstance()->Render();
    Character::GetInstance()->Render();
}

void GameScene::Logic(void)
{
    if(Character::GetInstance()->isAlive == false){
        Game::GetInstance()->SetActiveScene(static_cast<Scene*>(GameOverScene::GetInstance()));     
    }
    if(Map::GetInstance()->GetEnemyMap().size() == 0){
        GameOverScene::GetInstance()->isWin = true;
        Game::GetInstance()->SetActiveScene(static_cast<Scene*>(GameOverScene::GetInstance()));     
    }
    Map::GetInstance()->Update();
    Character::GetInstance()->Update();
}

void GameScene::Event(SDL_Event* userEvent)
{
    Character::GetInstance()->UserInput(userEvent);
    if(userEvent->type == SDL_KEYDOWN && userEvent->key.keysym.scancode == SDL_SCANCODE_ESCAPE && userEvent->key.repeat == 0){
        Game::GetInstance()->SetActiveScene(static_cast<Scene*>(PauseScene::GetInstance()));
    }
}

GameScene* GameScene::GetInstance(void)
{
    return &mInstance;
}

GameScene GameScene::mInstance;