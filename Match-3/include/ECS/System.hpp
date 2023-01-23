#ifndef __ECS_System_h__
#define __ECS_System_h__

#include <set>

#include "ECS/Definitions.hpp"

class System
{
public:
	// Entities which the system can affect --> if (EntitySignature && SystemSignature == SystemSignature)
	std::set<Entity> entities;

	// Init is used to set the system's signature (and initialize everything that's needed)
	virtual void Init() {};
};

#endif //__ECS_System_h__