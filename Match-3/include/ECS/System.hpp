#ifndef __ECS_System_h__
#define __ECS_System_h__

#include <set>

#include "ECS/Definitions.hpp"

class System
{
public:
	std::set<Entity> entities;
};

#endif //__ECS_System_h__