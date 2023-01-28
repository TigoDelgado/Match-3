#ifndef __Match3_Component_Translate_h__
#define __Match3_Component_Translate_h__

#include "Math2D.hpp"

struct Translate
{
    Vector2f destination;
    float speed;
    float acceleration;
    float maxSpeed;
};

#endif //__Match3_Component_Translate_h__