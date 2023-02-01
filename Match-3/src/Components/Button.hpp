#ifndef __Match3_Component_Button_h__
#define __Match3_Component_Button_h__

#include "Math2D.hpp"

enum ButtonType
{
    START,
    QUIT,
    PLUS,
    MINUS,
    BACK,
    DEFAULT
};

struct Button
{
    int index;
    ButtonType type;
    Vector2f dimensions;
};

#endif //__Match3_Component_Button_h__