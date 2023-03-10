#ifndef __ECS_Manager_h__
#define __ECS_Manager_h__

#include "ECS/Definitions.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/ComponentManager.hpp"
#include "ECS/SystemManager.hpp"

#include <iostream>

class ECS_Manager 
{
public:
    void Init()
	{
		entityManager = std::make_unique<EntityManager>();
		componentManager = std::make_unique<ComponentManager>();
		systemManager = std::make_unique<SystemManager>();
	}


    // Entity methods
	Entity CreateEntity()
	{
		return entityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		entityManager->DestroyEntity(entity);
		componentManager->EntityDestroyed(entity);
		systemManager->EntityDestroyed(entity);
	}


    // Component methods
	template<typename T> void RegisterComponent()
	{
		componentManager->RegisterComponent<T>();
	}

	template<typename T> void AddComponent(Entity entity, T component)
	{	
		componentManager->AddComponent<T>(entity, component);

		Signature signature = entityManager->GetSignature(entity);
		signature.set(componentManager->GetComponentType<T>(), true);
		entityManager->SetSignature(entity, signature);

		systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T> void RemoveComponent(Entity entity)
	{
		componentManager->RemoveComponent<T>(entity);

		auto signature = entityManager->GetSignature(entity);
		signature.set(componentManager->GetComponentType<T>(), false);
		entityManager->SetSignature(entity, signature);

		systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T> T& GetComponent(Entity entity)
	{
		return componentManager->GetComponent<T>(entity);
	}

	template<typename T> ComponentType GetComponentType()
	{
		return componentManager->GetComponentType<T>();
	}


    // System methods
	template<typename T> std::shared_ptr<T> RegisterSystem()
	{
		return systemManager->RegisterSystem<T>();
	}

	template<typename T> std::shared_ptr<T> GetSystem()
	{
		return systemManager->GetSystem<T>();
	}

	template<typename T> void SetSystemSignature(Signature signature)
	{
		systemManager->SetSignature<T>(signature);
	}


private:
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<ComponentManager> componentManager;
    std::unique_ptr<SystemManager> systemManager;
};

#endif //__ECS_Manager_h__