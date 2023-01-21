#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Game.hpp"
#include "RenderWindow.hpp"
#include "ECS/Manager.hpp"

#include "Components/Body.hpp"
#include "Components/Position.hpp"
#include "Components/Sprite.hpp"

#include "Systems/Gravity.hpp"

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

    /*
    -------------------------------- INIT ECS ------------------------------------
    */

    ecsManager.Init();

    ecsManager.RegisterComponent<Position>();
    ecsManager.RegisterComponent<Body>();
    ecsManager.RegisterComponent<Sprite>();

    auto gravitySystem = ecsManager.RegisterSystem<GravitySystem>();
	{
		Signature signature;
		signature.set(ecsManager.GetComponentType<Position>());
		signature.set(ecsManager.GetComponentType<Body>());
		ecsManager.SetSystemSignature<GravitySystem>(signature);
	}
 
	gravitySystem->Init();

    /* 
    ------------------------------ SET SCENE START ---------------------------------------    
    */

    Entity entity_1 = ecsManager.CreateEntity();
	ecsManager.AddComponent(entity_1, Position{1.0f, 1.0f});
    ecsManager.AddComponent(entity_1, Body{});

    float dt = 0.0f;

    /* 
    ------------------------------ SET SCENE END ----------------------------------------    
    */

    bool running = true;
    SDL_Event event;

    while (running) 
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
        }

        window.Clear();
        window.Render(background);
        window.Display();

        gravitySystem->Update(dt);

        auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
    }

    window.CleanUp();
    SDL_Quit();
}