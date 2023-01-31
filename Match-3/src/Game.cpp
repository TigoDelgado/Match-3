#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "Game.hpp"
#include "RenderWindow.hpp"
#include "Timer.hpp"

#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "Scenes/Level.hpp"

ECS_Manager ecsManager;
SceneManager sceneManager;

#define TIME_STEP 0.01f         // fixed dt for updates
#define STEPS_PER_FRAME 2       // max steps allowed in a single frame

Game::Game()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0)
        std::cout << "SDL_Init Failed. SDL_Error: " << SDL_GetError() << std::endl;

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        std::cout << "IMG_Init Failed. SDL_Error: " << SDL_GetError() << std::endl;

    if (TTF_Init() == -1)
        std::cout << "TTF_Init Failed. SDL_Error: " << SDL_GetError() << std::endl;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        std::cout << "SDL_mixer failed to initialize. SDL_mixer Error: " << Mix_GetError() << std::endl;

    Mix_Init(MIX_INIT_MP3);

}

void Game::Run()
{
    RenderWindow window("Match 3", 720, 720);

    ecsManager.Init();
    sceneManager.Init();

    // TODO create scene manager
    GameScene* currentScene = new Level(window, 8, 8, "../res/Background/bs7.png", "../res/Board.png");

    gameMusic.push_back(Mix_LoadMUS( "../res/Audio/Bumper Tag - John Deley.mp3"));
    gameMusic.push_back(Mix_LoadMUS( "../res/Audio/Covert Affair - Film Noire - Kevin MacLeod.mp3"));
    gameMusic.push_back(Mix_LoadMUS( "../res/Audio/Piano Store - Jimmy Fontanez_Media Right Productions.mp3"));

    int musicIndex = rand() % 3;
    
    Mix_PlayMusic( gameMusic[musicIndex], -1 );

    /* ----------------------------------- Main Loop ------------------------------------ */

    float timeAccumulator = 0.0f;
    float endTime = Timer::CurrentTime();      

    float fpsTimer = endTime;
    int fpsCounter = 0;

    bool running = true;
    SDL_Event event;

    while (running) 
    {
        float startTime = Timer::CurrentTime();
        float deltaTime = startTime - endTime;
        
        endTime = Timer::CurrentTime();

        timeAccumulator += deltaTime;

        if ( startTime > fpsTimer + 1.0f) // count fps in 1 sec (1000 ms)
        {
            // std::cout << "FPS: " << (fpsCounter) << std::endl;
            fpsTimer = Timer::CurrentTime(); 
            fpsCounter = 0;
        }
        fpsCounter++;

        int stepsCount = 0;
        while (timeAccumulator >= TIME_STEP && stepsCount < STEPS_PER_FRAME)
        {
            while (SDL_PollEvent(&event))
            {
                switch(event.type) 
                {
                case SDL_QUIT:
                    // Handles Alt-F4 on Windows and Command-Q on OSX
                    running = false;
                default:
                    break;
                }

                currentScene->HandleEvent(event);
            }

            currentScene->Update(TIME_STEP);
            timeAccumulator -= TIME_STEP;
            stepsCount++;
        }

        currentScene->Render();
    }

    window.CleanUp();
    SDL_Quit();
}