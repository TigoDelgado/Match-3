#ifndef __Match3_Entity_Default_h__
#define __Match3_Entity_Default_h__

#include "ECS/Manager.hpp"
#include "ECS/Definitions.hpp"

extern ECS_Manager ecsManager;

namespace DefaultEntity 
{
    Entity CreateEntity()
    {
        Entity entity = ecsManager.CreateEntity();

        // ecsManager.AddComponent(entity, Component{});

        return entity;
    };
};


#endif //__Match3_Entity_Default_h__