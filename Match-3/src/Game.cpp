#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Game.hpp"
#include "RenderWindow.hpp"

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

    bool running = true;
    SDL_Event event;

    while (running) 
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
        }

        window.Clear();
        window.Render(background);
        window.Display();
    }

    window.CleanUp();
    SDL_Quit();
}