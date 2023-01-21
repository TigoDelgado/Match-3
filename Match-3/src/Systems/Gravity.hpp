#ifndef __Match3_System_Gravity_h__
#define __Match3_System_Gravity_h__

#include "ECS/System.hpp"

class GravitySystem : public System
{
public:
    void Init();
	void Update(float dt);
private:
    float gravityForce = 0.1f;
};


#endif //__Match3_System_Gravity_h__