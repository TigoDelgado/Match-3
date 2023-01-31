#ifndef __Match3_System_Delay_Animation_h__
#define __Match3_System_Delay_Animation_h__

#include "ECS/System.hpp"
#include "Math2D.hpp"

class DelayAnimationSystem : public System
{
public:
    void Init() override;
	void Update(float dt);

private:
};


#endif //__Match3_System_Delay_Animation_h__