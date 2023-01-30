#ifndef __Match3_Component_Sprite_h__
#define __Match3_Component_Sprite_h__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math2D.hpp"

struct Sprite
{
    SDL_Texture* texture;
    int currentFrame;
    Vector2f sourceDimensions;
    Vector2f destDimensions;
};

#endif //__Match3_Component_Sprite_h__