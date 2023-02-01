#ifndef __ECS_Component_Manager_h__
#define __ECS_Component_Manager_h__

#include <unordered_map>
#include <memory>

#include "ECS/Definitions.hpp"
#include "ECS/ComponentArray.hpp"

class ComponentManager
{
public:
	template<typename T> void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

        if (componentTypes.find(typeName) != componentTypes.end())
		{
			throw std::invalid_argument("Component already registered.");
		}

		componentTypes.insert({typeName, activeComponentTypes});

		componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		++activeComponentTypes;
	}

	template<typename T> ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

        if (componentTypes.find(typeName) == componentTypes.end())
		{
			throw std::invalid_argument("Component is not registered.");
		}


		return componentTypes[typeName];
	}

    template<typename T> void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}

    template<typename T> void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

    template<typename T> T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

    void EntityDestroyed(Entity entity)
	{
		// Notify each component array that an entity has been destroyed
		for (auto const& pair : componentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}


private:
	// Map from type string pointer to a component type
	std::unordered_map<const char*, ComponentType> componentTypes{};

	// Map from type string pointer to a component array
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};

    // Count of active component types --> must be lower than MAX_COMPONENTS
	ComponentType activeComponentTypes{};

    // Convenience function to get the statically casted pointer to the ComponentArray of type T.
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

        if (componentTypes.find(typeName) == componentTypes.end())
		{
			throw std::invalid_argument("Component is not registered.");
		}

		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
	}
};


#endif //__ECS_Component_Manager_h__