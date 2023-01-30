#include "ECS/Manager.hpp"
#include "Systems/DestroyEntity.hpp"

#include "Components/EntityDestruction.hpp"

#include "Math2D.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

void DestroyEntitySystem::Init()
{
	Signature signature;
    signature.set(ecsManager.GetComponentType<EntityDestruction>());

	ecsManager.SetSystemSignature<DestroyEntitySystem>(signature);
}


bool DestroyEntitySystem::Update(float dt)
{
	if (entities.size() <= 0) return false;

	for (Entity const& entity : entities)
	{
		EntityDestruction& destruction = ecsManager.GetComponent<EntityDestruction>(entity);

		if (destruction.timeLeft <= 0)
		{
			// std::cout << "Destroyed entity: " << entity  << std::endl;
			ecsManager.DestroyEntity(entity);
		}
		else		// wait to destroy entity
		{
			destruction.timeLeft -= dt;
		}
	}

	return true;
}