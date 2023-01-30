#ifndef __Match3_Component_Sprite_Animation_h__
#define __Match3_Component_Sprite_Animation_h__

#include "Math2D.hpp"
#include <vector>

struct SpriteAnimation
{
    std::vector<int> frames;        // sprite sheet frames to iterate through - including initial frame
    int loops = -1;                 // how many loops to animate before terminating (-1 if infinite loop)
};

#endif //__Match3_Component_Sprite_Animation_h__