#ifndef __Match3_Game_h__
#define __Match3_Game_h__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "ECS/Manager.hpp"

class Game
{
public:
    Game();
    void Run();
private:
    void RegisterComponents();
    void RegisterSystems();
    std::vector<Mix_Music*> gameMusic;
};

#endif //__Match3_Game_h__