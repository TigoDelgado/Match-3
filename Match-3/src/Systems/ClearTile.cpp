#include "ECS/Manager.hpp"
#include "Systems/ClearTile.hpp"

#include "Components/Transform.hpp"
#include "Components/TileObject.hpp"
#include "Components/Destruction.hpp"

#include "Math2D.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

void ClearTileSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
    signature.set(ecsManager.GetComponentType<TileObject>());
    signature.set(ecsManager.GetComponentType<Destruction>());

	ecsManager.SetSystemSignature<ClearTileSystem>(signature);
}


bool ClearTileSystem::Update(float dt)
{
	if (entities.size() <= 0) return false;

	for (Entity const& entity : entities)
	{
		Transform& transform = ecsManager.GetComponent<Transform>(entity);
		// TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);
        Destruction& destruction = ecsManager.GetComponent<Destruction>(entity);

		if (transform.scale <= destruction.scale)
		{
			// std::cout << "DESTROYED entity previously on [" << tileObject.coords.x << ", " << tileObject.coords.y << "]" << std::endl;
			ecsManager.DestroyEntity(entity);
		}
		else
		{
			transform.scale -= destruction.speed * dt;
		}
	}

	return true;
}