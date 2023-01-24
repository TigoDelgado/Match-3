#ifndef __Match3_Timer_h__
#define __Match3_Timer_h__

#include <SDL2/SDL.h>

namespace Timer
{
    float CurrentTime()
    {
        //return time in seconds
        float time = SDL_GetTicks();
        time *= 0.001f;
        return time;
    }
}



#endif //__Match3_Timer_h__