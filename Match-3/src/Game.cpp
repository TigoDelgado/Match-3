#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Game.hpp"
#include "RenderWindow.hpp"
#include "ECS/Manager.hpp"

#include "GameScene.hpp"
#include "Scenes/Level.hpp"

ECS_Manager ecsManager;

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

    float dt = 0.0f;
    bool running = true;
    SDL_Event event;

    while (running) 
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;

            currentScene->HandleEvent(event);
        }

        currentScene->Update(dt);
        
        currentScene->Render();

        auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
    }

    window.CleanUp();
    SDL_Quit();
}