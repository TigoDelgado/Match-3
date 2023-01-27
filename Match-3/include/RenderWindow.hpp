#ifndef __Match3_RenderWindow_h__
#define __Match3_RenderWindow_h__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math2D.hpp"

class RenderWindow
{
public:
    RenderWindow(const char* p_title, int p_w, int p_h);
    SDL_Texture* LoadTexture(const char* p_filePath);
    void CleanUp();
    void Clear();
    void Render(SDL_Texture* p_texture, Vector2f p_position);
    void Render(SDL_Texture* p_texture, Vector2f p_position, Vector2f p_dimensions, double p_rotation, SDL_Point* p_center, SDL_RendererFlip p_flip);
    void Display();
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif //__Match3_RenderWindow_h__