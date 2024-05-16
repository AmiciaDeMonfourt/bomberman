#include "menu_scene.hpp"
#include "game_scene.hpp"
#include "../game.hpp"
#include "../utils/timer.hpp"
#include "../utils/texture_map.hpp"
#include "../character.hpp"
#include "../map.hpp"


void MenuScene::CleanUp(void)
{
    std::cout << "MainScene::CleanUp - begin" << std::endl;
    if(mButtons != nullptr)
    {
        for(int i = 0; i < sizeof(mButtons) / sizeof(mButtons[0]); i++)
            delete mButtons[i];

        delete mButtons;
    }
    std::cout << "MainScene::CleanUp - end" << std::endl;
}

void MenuScene::Init(void)
{
    const int numButtons = 3;

    mButtons = new Button*[numButtons];

    SDL_FRect bBox = {580, 200, 100, 100};
    mButtons[bPlay] = new Button("Играть", bBox, Consts::WHITE, Consts::GREEN);

    bBox = {580, 300, 100, 100};
    mButtons[bAuthor] = new Button("Автор", bBox, Consts::WHITE, Consts::GREEN);

    bBox = {580, 400, 100, 100};
    mButtons[bHelp] = new Button("Справка", bBox, Consts::WHITE, Consts::GREEN);

    bBox = {580, 500, 100, 100};
    mButtons[bExit] = new Button("Выход", bBox, Consts::WHITE, Consts::GREEN);

    bBox = {580, 770, 100, 100};
    mButtons[bBack] = new Button("Назад", bBox, Consts::WHITE, Consts::GREEN);

    titleFont = TTF_OpenFont("font/glastown.ttf", 128);

    state = sTitle;
}

void MenuScene::Render(void)
{
    static int srcX = 0;
    static float timeToUpdate = 0;

    timeToUpdate += Timer::GetDt();
    // std::cout << timeToUpdate << std::endl;
    if(timeToUpdate > 1.8)
    {
        srcX = (srcX + 64) % 256;
        timeToUpdate = 0;
    }

    SDL_FRect hitBox = {-256, 0, 256, 256};

    SDL_Rect spriteBox  = {srcX, 0, 64, 64};

    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            hitBox.x += 256;
            TextureMap::Get("title")->Render(hitBox, spriteBox);
        }
        hitBox.x = -256;
        hitBox.y += 256;
    }

    
    

    switch(state)
    {
        case sTitle:
        {
            SDL_Point pos = {330, 80};
            RenderText("Игра Подрывник", titleFont, pos, Consts::WHITE);
            mButtons[bPlay]->Render();
            mButtons[bAuthor]->Render();
            mButtons[bHelp]->Render();
            mButtons[bExit]->Render();
            break;
        }
        

        case sAuthor:
        {
            std::string text[] = 
            {
                "Технические требования", 
                "-Возможность управления персонажем по игровому полю;", 
                "-Возможность установки бомб на игровом поле.",
                "-Возможность получения усилений для игрового персонажа;",
                "-Автор: Рожкова К.(?). А821С"
            }; 
            
            SDL_Point pos = {440, 200};
            SDL_Color color = Consts::WHITE;

            RenderText(text[0], pos, color);
            for(int i = 1; i < 4; i++){
                pos = {280, 200 + i*50};
                RenderText(text[i], pos, color);
            }
            
            pos = {420, 450};
            RenderText(text[4], pos, color);
            
            mButtons[bBack]->Render();
            break;
        }
            
        
        
        case sHelp:
        {
            std::string text[] = 
            {
                "Игра Подрывник",
                "Игрок управляет персонажем, находящимся в лабиринте, состоящем из разрушаемых",
                "и не разрушаемых стен. Он может оставлять бомбу (кнопка Space), взрывающуюся ",
                "через некоторое фиксированное время и разрушающую стены попавшие под взрыв.",
                "Специальные бонусы:",
                "- Увеличение количества одновременно поставленных бомб;",
                "- Увеличение дальности их взрыва;", 
                "- Увеличение скорости перемещения героя;", 
                "- Дает возможность взрыва бомб по нажатию на кнопку 'И';",
                "- Дает возможность пройти сквозь еще не взорванные бомбы.",
                "- Дает невоспреимчемость от взрыва бомб;", 
                " На уровне присутствуют противники."
            };

            SDL_Point pos = {500, 50};
            SDL_Color color = Consts::WHITE;

            RenderText(text[0], pos, color);
            for(int i = 1; i < 4; i++){
                pos = {100, 50 + i*50};
                RenderText(text[i], pos, color);
            }

            pos = {480, 250};
            
            RenderText(text[4], pos, color);
            for(int i = 5; i < 11; i++)
            {
                pos = {240, 50 + i*50};
                RenderText(text[i], pos, color);
                SDL_FRect hitBox = {pos.x - 60.f, pos.y*1.f, 50.f, 50.f};
                SDL_Rect srcBox = {16 * (i-5), 0, 16, 16};
                TextureMap::Get("boosts")->Render(hitBox, srcBox);
            }

            pos = {pos.x, pos.y + 50};
            RenderText(text[11], pos, color);

            mButtons[bBack]->Render();
            break;
        }
    }
}

//============================================================================//

void MenuScene::Logic(void)
{
    switch(state)
    {
        case sTitle:
            
            if(mButtons[bPlay]->GetIsClick())
            {
                Map::GetInstance()->Init();
                Character::GetInstance()->Init();
                Game::GetInstance()->SetActiveScene(static_cast<Scene*>(GameScene::GetInstance()));
                mButtons[bPlay]->Reset();
            }

            else if(mButtons[bAuthor]->GetIsClick())
            {
                state = sAuthor;
                mButtons[bAuthor]->Reset();
            }

            else if(mButtons[bHelp]->GetIsClick())
            {
                state = sHelp;
                mButtons[bHelp]->Reset();
            }

            else if(mButtons[bExit]->GetIsClick()) 
            {
                Game::GetInstance()->SetRunning(false);
                mButtons[bExit]->Reset();
            }
            
            break;

        case sAuthor:
            if(mButtons[bBack]->GetIsClick())
            {
                state = sTitle;
                mButtons[bBack]->Reset();
            }
            break;
        

        case sHelp:
            if(mButtons[bBack]->GetIsClick())
            {
                state = sTitle;
                mButtons[bBack]->Reset();
            }
            break;
    }

}  

//============================================================================//

void MenuScene::Event(SDL_Event *userEvent)
{
    switch(state)
    {
        case sTitle:
            mButtons[bPlay]->UserInput(userEvent);
            mButtons[bAuthor]->UserInput(userEvent);
            mButtons[bHelp]->UserInput(userEvent);
            mButtons[bExit]->UserInput(userEvent);
            break;
        
        case sAuthor:
            mButtons[bBack]->UserInput(userEvent);
            break;
        
        case sHelp:
            mButtons[bBack]->UserInput(userEvent);
            break;
    }
}

//============================================================================//

MenuScene* MenuScene::GetInstance(void)
{
    return &mInstance;
}

MenuScene MenuScene::mInstance;