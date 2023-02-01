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


    globalFont = TTF_OpenFont( "../res/Fonts/OpenSans/OpenSans-Bold.ttf", 36 );
    if (globalFont == NULL)
        std::cout << "Failed to open global font. SDL_Error: " << SDL_GetError() << std::endl;
}

SDL_Texture* RenderWindow::LoadTexture(const char* p_filePath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL)
        std::cout << "Failed to load texture from " << (p_filePath) << ". SDL_Error: " << SDL_GetError() <<std::endl;

    return texture;
}

SDL_Texture* RenderWindow::LoadTextureFromText(const char* p_text, SDL_Color p_color)
{
    // Render text surface
    SDL_Texture* texture = NULL;

    SDL_Surface* textSurface = TTF_RenderText_Blended( globalFont, p_text, p_color );
    if( textSurface == NULL )
    {
        std::cout << "Failed to render text surface. SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface( renderer, textSurface );

        if( texture == NULL )
        {
            std::cout << "Failed to create texture from rendered text. SDL_Error: " << SDL_GetError() << std::endl;
        }

        // Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
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

void RenderWindow::Render(SDL_Texture* p_texture, Vector2f p_position)
{
    int textureWidth, textureHeight;
    SDL_QueryTexture(p_texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = textureWidth;
    src.h = textureHeight;

    SDL_Rect dst;
    dst.x = p_position.x;
    dst.y = p_position.y;
    dst.w = textureWidth;
    dst.h = textureHeight;

    SDL_RenderCopy(renderer, p_texture, &src, &dst);
}

void RenderWindow::Render(SDL_Texture* p_texture, Vector2f p_position, Vector2f p_sheetOffset, Vector2f p_sourceDimensions, Vector2f p_destDimensions)
{
    int textureWidth, textureHeight;
    SDL_QueryTexture(p_texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect src;
    src.x = p_sheetOffset.x;
    src.y = p_sheetOffset.y;
    src.w = p_sourceDimensions.x;
    src.h = p_sourceDimensions.y;

    SDL_Rect dst;
    dst.x = p_position.x;
    dst.y = p_position.y;
    dst.w = p_destDimensions.x;
    dst.h = p_destDimensions.y;

    SDL_RenderCopy(renderer, p_texture, &src, &dst);
}

void RenderWindow::Render(SDL_Texture* p_texture, Vector2f p_position, Vector2f p_sheetOffset, Vector2f p_sourceDimensions, Vector2f p_destDimensions, double p_rotation, SDL_Point* p_center, SDL_RendererFlip p_flip)
{
    SDL_Rect src;
    src.x = p_sheetOffset.x;
    src.y = p_sheetOffset.y;
    src.w = p_sourceDimensions.x;
    src.h = p_sourceDimensions.y;

    SDL_Rect dst;
    dst.x = p_position.x;
    dst.y = p_position.y;
    dst.w = p_destDimensions.x;
    dst.h = p_destDimensions.y;

    SDL_RenderCopyEx(renderer, p_texture, &src, &dst, p_rotation, p_center, p_flip);
}

void RenderWindow::Display()
{
    SDL_RenderPresent(renderer);
}

Vector2f RenderWindow::GetDimensions()
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return Vector2f{float(w), float(h)};
}