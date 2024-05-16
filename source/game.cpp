#include "game.hpp"
#include "utils/timer.hpp"
#include "scenes/menu_scene.hpp"
#include "scenes/game_scene.hpp"
#include "scenes/pause_scene.hpp"
#include "scenes/game_over_scene.hpp"
#include "utils/texture_map.hpp"
#include "map.hpp"


void Game::Init(void)
{
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
        throw std::runtime_error(std::string("SDL not init: ") + SDL_GetError());
    

    if(IMG_Init(IMG_INIT_PNG) < 0)
        throw std::runtime_error(std::string("IMG not init: ") + SDL_GetError());


    if(TTF_Init() < 0)
        throw std::runtime_error(std::string("TTF not init: ") + SDL_GetError());


    if(Mix_Init(MIX_INIT_MP3) < 0)
        throw std::runtime_error(std::string("MIX not init: ") + SDL_GetError());        


    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        throw std::runtime_error(std::string("MixOpenAudio: ") + SDL_GetError());


    if(SDL_SetHint(SDL_HINT_RENDER_VSYNC, "true") == SDL_FALSE)
        throw std::runtime_error(std::string("SetHing error: ") + SDL_GetError());
    

    Uint32 windowFlags = SDL_WINDOW_SHOWN;
    mWindow = SDL_CreateWindow(
                                    "Бархатные тяги",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    Consts::WINDOW_WIDTH,
                                    Consts::WINDOW_HEIGHT,
                                    windowFlags
                              );
    if(mWindow == nullptr) 
        throw std::runtime_error(std::string("Window == nullptr: ") + SDL_GetError());
    

    Uint32 rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    mRenderer = SDL_CreateRenderer(
                                        mWindow,
                                        -1, 
                                        rendererFlags
                                  );

    if(mWindow == nullptr) 
        throw std::runtime_error(std::string("Renderer == nullptr: ") + SDL_GetError());
    
    mActiveScene = MenuScene::GetInstance();
    if(mActiveScene == nullptr)
        throw std::runtime_error(std::string("mActiveScene == nullptr: ") + SDL_GetError());

    mScreenTextFont = TTF_OpenFont("font/glastown.ttf", 48);
    if(mScreenTextFont == nullptr)
        throw std::runtime_error(std::string("mScreenText == nullptr: ") + SDL_GetError());
    
    
    TextureMap::Init();
    
    MenuScene::GetInstance()->Init();

    PauseScene::GetInstance()->Init();

    GameOverScene::GetInstance()->Init();
        
    Timer::Start();

    mRunning = true;
}


void Game::Loop(void)
{
    SDL_Event event;
    
    while(mRunning)
    {
        SDL_RenderClear(mRenderer);

        Timer::CalculateDt();        

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                mRunning = false;

            mActiveScene->Event(&event);
        }
        
        mActiveScene->Logic();      

        mActiveScene->Render();
       
        SDL_RenderPresent(mRenderer);
    }
}


Game::~Game()
{
    std::cout << "~Game - begin" << std::endl;

    TextureMap::CleanUp();
    Map::GetInstance()->CleanUp();
    MenuScene::GetInstance()->CleanUp();
    PauseScene::GetInstance()->CleanUp();
    GameOverScene::GetInstance()->CleanUp();
    
    SDL_DestroyRenderer(mRenderer);

    SDL_DestroyWindow(mWindow);

    TTF_CloseFont(mScreenTextFont);

    Mix_CloseAudio();
    
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    std::cout << "~Game - end" << std::endl;
}



SDL_Window* Game::GetWindow(void) const { return mWindow; }

SDL_Renderer* Game::GetRenderer(void) const { return mRenderer; }

TTF_Font* Game::GetScreenTextFont(void) const { return mScreenTextFont; }

Game* Game::GetInstance(void) { return &mInstance; }

void Game::SetActiveScene(Scene *newScene) { mActiveScene = newScene;}

void Game::SetRunning(bool state) { mRunning = state; }

Game Game::mInstance;


