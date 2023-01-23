#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Entities/Gem.hpp"

#include "Game.hpp"
#include "RenderWindow.hpp"
#include "ECS/Manager.hpp"

#include "Components/Body.hpp"
#include "Components/Position.hpp"
#include "Components/Sprite.hpp"

#include "Systems/Gravity.hpp"
#include "Systems/Render.hpp"

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

    SDL_Texture* background = window.LoadTexture("res/Backdrop13.jpg");

    ecsManager.Init();


    /* ------------------------------ Register Components ------------------------------ */
    
    ecsManager.RegisterComponent<Position>();
    ecsManager.RegisterComponent<Body>();
    ecsManager.RegisterComponent<Sprite>();


    /* -------------------------------- Register Systems -------------------------------- */

    std::shared_ptr<GravitySystem> gravitySystem = ecsManager.RegisterSystem<GravitySystem>();
    std::shared_ptr<RenderSystem> renderSystem = ecsManager.RegisterSystem<RenderSystem>();



    /* ---------------------------------- Create Scene ---------------------------------- */
    
    SDL_Texture* gemBlackTexture = window.LoadTexture("res/Color-1.png");
    SDL_Texture* gemWhiteTexture = window.LoadTexture("res/Color-2.png");
    SDL_Texture* gemPinkTexture = window.LoadTexture("res/Color-3.png");
    SDL_Texture* gemBlueTexture = window.LoadTexture("res/Color-4.png");
    SDL_Texture* gemOrangeTexture = window.LoadTexture("res/Color-5.png");

    Gem::CreateEntity(480.0f, 0.0f, gemBlackTexture);
    Gem::CreateEntity(540.0f, 0.0f, gemWhiteTexture);
    Gem::CreateEntity(600.0f, 0.0f, gemPinkTexture);
    Gem::CreateEntity(660.0f, 0.0f, gemBlueTexture);
    Gem::CreateEntity(720.0f, 0.0f, gemOrangeTexture);

    

    /* ----------------------------------- Game Loop ------------------------------------ */

    float dt = 0.0f;
    bool running = true;
    SDL_Event event;

    while (running) 
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
        }

        Gem::CreateEntity(480.0f, 0.0f, gemBlackTexture);
        Gem::CreateEntity(540.0f, 0.0f, gemWhiteTexture);
        Gem::CreateEntity(600.0f, 0.0f, gemPinkTexture);
        Gem::CreateEntity(660.0f, 0.0f, gemBlueTexture);
        Gem::CreateEntity(720.0f, 0.0f, gemOrangeTexture);
        
        gravitySystem->Update(dt);

        window.Clear();
        window.Render(background, .0f, .0f, 1280, 720);
        renderSystem->Update(window);
        window.Display();

        auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
    }

    window.CleanUp();
    SDL_Quit();
}