#ifndef __Match3_System_Gravity_h__
#define __Match3_System_Gravity_h__

#include "ECS/System.hpp"

class GravitySystem : public System
{
public:
    void Init() override;
	void Update(float dt);
private:
    float gravityForce = 20.0f;
};


#endif //__Match3_System_Gravity_h__