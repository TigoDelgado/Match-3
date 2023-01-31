#ifndef __Match3_Component_Sprite_Animation_h__
#define __Match3_Component_Sprite_Animation_h__

#include "Math2D.hpp"
#include "GameDefinitions.hpp"
#include <vector>

struct SpriteAnimation
{
    int firstFrame;             // sprite sheet frames to iterate through - including initial frame
    int lastFrame;
    int currentFrame;
    int returnFrame; 
    int loops;                 // how many loops to animate before terminating (-1 if infinite loop)
    float frameTime;                // how long each frame takes
    float timeCount;
};

#endif //__Match3_Component_Sprite_Animation_h__