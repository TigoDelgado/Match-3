#ifndef __Match3_RenderWindow_h__
#define __Match3_RenderWindow_h__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Math2D.hpp"

// RenderWindow handles the SDL_Window and SDL_Renderer
// Takes care of rendering textures through SDL_RenderCopy

class RenderWindow
{
public:
    RenderWindow(const char* p_title, int p_w, int p_h);
    SDL_Texture* LoadTexture(const char* p_filePath);
    SDL_Texture* LoadTextureFromText(const char* p_text, SDL_Color p_color);
    void CleanUp();
    void Clear();
    void Render(SDL_Texture* p_texture, Vector2f p_position);
    void Render(SDL_Texture* p_texture, Vector2f p_position, Vector2f p_offset, Vector2f p_sourceDimensions, Vector2f p_destDimensions);
    void Render(SDL_Texture* p_texture, Vector2f p_position, Vector2f p_offset, Vector2f p_sourceDimensions, Vector2f p_destDimensions, double p_rotation, SDL_Point* p_center, SDL_RendererFlip p_flip);
    void Display();
    Vector2f GetDimensions();
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* globalFont;
};

#endif //__Match3_RenderWindow_h__