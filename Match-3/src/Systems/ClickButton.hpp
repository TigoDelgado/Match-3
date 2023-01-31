#ifndef __Match3_System_Click_Button_h__
#define __Match3_System_Click_Button_h__

#include "ECS/System.hpp"
#include "Math2D.hpp"

class ClickButtonSystem : public System
{
public:
    void Init() override;
	int ClickedButton(Vector2f p_point);
};


#endif //__Match3_System_Click_Button_h__