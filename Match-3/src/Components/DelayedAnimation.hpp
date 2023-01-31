#ifndef __Match3_Component_Delayed_Animation_h__
#define __Match3_Component_Delayed_Animation_h__

#include "Math2D.hpp"
#include "GameDefinitions.hpp"
#include "Components/SpriteAnimation.hpp"

#include <vector>

struct DelayedAnimation
{
    SpriteAnimation animation;
    float delay;
    float timeCount;
    float loops;
};

#endif //__Match3_Component_Delayed_Animation_h__