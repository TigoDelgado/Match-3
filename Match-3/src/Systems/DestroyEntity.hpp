#ifndef __Match3_System_Destroy_Entity_h__
#define __Match3_System_Destroy_Entity_h__

#include "ECS/System.hpp"
#include "Math2D.hpp"

class DestroyEntitySystem : public System
{
public:
    void Init() override;
	bool Update(float dt);
};


#endif //__Match3_System_Destroy_Entity_h__