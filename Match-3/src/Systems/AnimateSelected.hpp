#ifndef __Match3_System_Animate_Selected_h__
#define __Match3_System_Animate_Selected_h__

#include "ECS/System.hpp"
#include "Math2D.hpp"

class AnimateSelectedSystem : public System
{
public:
    void Init() override;
	void Update(float dt);

private:
};


#endif //__Match3_System_Animate_Selected_h__