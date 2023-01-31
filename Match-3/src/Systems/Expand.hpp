#ifndef __Match3_System_Expand_h__
#define __Match3_System_Expand_h__

#include "ECS/System.hpp"
#include "Math2D.hpp"

class ExpandSystem : public System
{
public:
    void Init() override;
	bool Update(float dt);
};


#endif //__Match3_System_Expand_h__