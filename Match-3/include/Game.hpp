#ifndef __Match3_Game_h__
#define __Match3_Game_h__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "ECS/Manager.hpp"
#include "GameScene.hpp"

class Game
{
public:
    Game();
    void Run();
private:
    GameScene* currentScene;
    GameScene* nextScene;
};

#endif //__Match3_Game_h__