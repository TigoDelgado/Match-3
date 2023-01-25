#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Game.hpp"
#include "RenderWindow.hpp"
#include "Timer.hpp"

#include "GameScene.hpp"
#include "Scenes/Level.hpp"

ECS_Manager ecsManager;

#define TIME_STEP 0.01f         // fixed dt for updates
#define STEPS_PER_FRAME 2       // max steps allowed in a single frame

Game::Game()
{
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
        std::cout << "SDL_Init FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) //FIXME don't use jpegs please :S
        std::cout << "IMG_Init FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;

}

void Game::Run()
{
    RenderWindow window("Match 3", 1280, 720);

    ecsManager.Init();

    // TODO create scene manager
    GameScene* currentScene = new Level(window, 8, 8, "res/Backdrop13.jpg");


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
            switch(event.type) {
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