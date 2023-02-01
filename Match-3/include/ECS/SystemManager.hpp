#ifndef __ECS_System_Manager_h__
#define __ECS_System_Manager_h__

#include <set>
#include <memory>
#include <unordered_map>

#include "ECS/Definitions.hpp"
#include "ECS/System.hpp"

class SystemManager
{
public:

	template<typename T> std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		if (systems.find(typeName) != systems.end())
		{
			std::invalid_argument("System is already registered.")
		}

		// Create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>();
		systems.insert({typeName, system});

		system->Init();

		return system;
	}

	template<typename T> std::shared_ptr<T> GetSystem()
	{
		const char* typeName = typeid(T).name();

		auto system = systems.at(typeName);

		return system;
	}



	template<typename T> void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		if (systems.find(typeName) == systems.end())
		{
			std::invalid_argument("System is not registered.")
		}

		signatures.insert({typeName, signature});
	}

    void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : systems)
		{
			auto const& system = pair.second;

			system->entities.erase(entity);
		}
	}

    void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed and check if entity is still part of system
		for (auto const& pair : systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = signatures[type];

			if ((entitySignature & systemSignature) == systemSignature) system->entities.insert(entity);

			else system->entities.erase(entity);
		}
	}


private:
    // Each system type has a signature which can be compared to Entity Signatures
	std::unordered_map<const char*, Signature> signatures{};

    // Map from system type string pointer to a system pointer
	std::unordered_map<const char*, std::shared_ptr<System>> systems{};
};


#endif //__ECS_System_Manager_h__