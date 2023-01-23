#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_width, int p_height)
    :window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_width, p_height, SDL_WINDOW_SHOWN);

    if (window == NULL)
        std::cout << "Failed to create WINDOW. SDL_Error: " << SDL_GetError() << std::endl;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL)
        std::cout << "Failed to create RENDERER. SDL_Error: " << SDL_GetError() << std::endl;
}

SDL_Texture* RenderWindow::LoadTexture(const char* p_filePath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL)
        std::cout << "Failed to load texture from " << (p_filePath) << ". SDL_Error: " << SDL_GetError() <<std::endl;

    return texture;
}

void RenderWindow::CleanUp()
{
    SDL_DestroyWindow(window);
}

void RenderWindow::Clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::Render(SDL_Texture* p_texture, float p_x, float p_y)
{
    int textureWidth, textureHeight;
    SDL_QueryTexture(p_texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = textureWidth;
    src.h = textureHeight;

    SDL_Rect dst;
    dst.x = p_x;
    dst.y = p_y;
    dst.w = 45;
    dst.h = 45;

    SDL_RenderCopy(renderer, p_texture, &src, &dst);
}

void RenderWindow::Render(SDL_Texture* p_texture, float p_x, float p_y, float p_w, float p_h)
{
    int textureWidth, textureHeight;
    SDL_QueryTexture(p_texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = textureWidth;
    src.h = textureHeight;

    SDL_Rect dst;
    dst.x = p_x;
    dst.y = p_y;
    dst.w = p_w;
    dst.h = p_h;

    SDL_RenderCopy(renderer, p_texture, &src, &dst);
}

void RenderWindow::Display()
{
    SDL_RenderPresent(renderer);
}