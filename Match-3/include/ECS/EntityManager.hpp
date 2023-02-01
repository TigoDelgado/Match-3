#ifndef __ECS_Entity_Manager_h__
#define __ECS_Entity_Manager_h__

#include <cstdint>
#include <queue>
#include <array>

#include "ECS/Definitions.hpp"

class EntityManager
{
public:
    EntityManager()
    {
        // Populate available entities with incremental ID's
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			availableEntities.push(entity);
		}
    }

    Entity CreateEntity()
	{
        if (activeEntities >= MAX_ENTITIES) 
        {
            throw std::out_of_range("Too many entities created");
        }
        
        Entity entity = availableEntities.front();
        availableEntities.pop();
        ++activeEntities;
        return entity;
	}

    void DestroyEntity(Entity entity)
    {
        if (entity >= MAX_ENTITIES) 
        {
            throw std::out_of_range("Entity index is out of range.");
        }

        entitySignatures[entity].reset();
        availableEntities.push(entity);
        --activeEntities;
    }

    void SetSignature(Entity entity, Signature signature)
    {
        if (entity >= MAX_ENTITIES)
        {
            throw std::out_of_range("Entity index is out of range.");
        }

        entitySignatures[entity] = signature;
    }

    Signature GetSignature(Entity entity)
    {
        if (entity >= MAX_ENTITIES)
        {
            throw std::out_of_range("Entity index is out of range.");
        }

        return entitySignatures[entity];
    }


private:
    // Queue of available entity IDs --> new entities get next available ID
    std::queue<Entity> availableEntities{};

    // Array of entity signatures --> index is the entity's ID
	std::array<Signature, MAX_ENTITIES> entitySignatures{};

    // Count of active entities --> must be lower than MAX_ENTITIES
	Entity activeEntities{};
};


#endif //__ECS_Entity_Manager_h__