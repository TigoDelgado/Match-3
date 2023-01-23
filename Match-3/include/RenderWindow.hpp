#ifndef __Match3_RenderWindow_h__
#define __Match3_RenderWindow_h__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderWindow
{
public:
    RenderWindow(const char* p_title, int p_w, int p_h);
    SDL_Texture* LoadTexture(const char* p_filePath);
    void CleanUp();
    void Clear();
    void Render(SDL_Texture* p_texture, float p_X, float p_y);
    void Render(SDL_Texture* p_texture, float p_X, float p_y, float p_w, float p_h);
    void Display();
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif //__Match3_RenderWindow_h__