/*
    - The ECS Manager will store one Component Array for each component type.
    - Component Arrays are arrays of Entities (ID's) which have a component of said type.
    - They must be kept packed in order to efficiently iterate over all indices.
    - Each time an entity is deleted (or loses its component), the array re-arranges itself in order to stay packed.
    - Two maps store the relation between Array Index and Entity (ID) to keep track of which array index stores an entity's data
    and which entity is present in each array index.
*/

#ifndef __ECS_Component_Array_h__
#define __ECS_Component_Array_h__

#include <iostream>

#include "ECS/Definitions.hpp"

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};


template<typename T> class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity entity, T component)
    {
        // FIXME: check if entity alread has component!

        // Store new entity in available index and update maps
		size_t newIndex = arraySize;
		entityToIndexMap[entity] = newIndex;
		indexToEntityMap[newIndex] = entity;
		componentArray[newIndex] = component;

		++arraySize;
    }

    void RemoveData(Entity entity)
	{
        // FIXME: check if component exists!

		// Copy element at end into deleted element's place to maintain density
		size_t entityIndex = entityToIndexMap[entity];
		size_t lastIndex = arraySize - 1;
		componentArray[entityIndex] = componentArray[lastIndex];

		// Update map to point to moved spot

        Entity lastEntity = indexToEntityMap[lastIndex];

		entityToIndexMap[lastEntity] = entityIndex;
		indexToEntityMap[entityIndex] = lastEntity;

		entityToIndexMap.erase(entity);
		indexToEntityMap.erase(lastIndex);

		--arraySize;
	}


    T& GetData(Entity entity)
	{
        //FIXME check if component exists

		// Return a reference to the entity's component
		return componentArray[entityToIndexMap[entity]];
	}


    void EntityDestroyed(Entity entity) override
	{
		if (entityToIndexMap.find(entity) != entityToIndexMap.end())
		{
			// Remove the entity's component if it existed
			RemoveData(entity);
		}
	}

private:
    // Array of entity ID's, storing which entities have component of type T
    std::array<T, MAX_ENTITIES> componentArray{};

    // Map used to keep track of which array index stores an entity's data
    std::unordered_map<Entity, size_t> entityToIndexMap{};

    // Map used to keep track of which entity has its data stored in each array index
    std::unordered_map<size_t, Entity> indexToEntityMap{};

    // Number of valid entity ID's present in the component array
    size_t arraySize;
};


#endif //__ECS_Component_Array_h__