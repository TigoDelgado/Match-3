#include "ECS/Manager.hpp"
#include "Systems/ClearTile.hpp"

#include "Components/Transform.hpp"
#include "Components/Expansion.hpp"
#include "Components/TileObject.hpp"
#include "Components/EntityDestruction.hpp"

#include "Math2D.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

void ClearTileSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
    signature.set(ecsManager.GetComponentType<Expansion>());
    signature.set(ecsManager.GetComponentType<TileObject>());

	ecsManager.SetSystemSignature<ClearTileSystem>(signature);
}


bool ClearTileSystem::Update(float dt)
{
	if (entities.size() <= 0) return false;

	for (Entity const& entity : entities)
	{
		Transform& transform = ecsManager.GetComponent<Transform>(entity);
        Expansion& expansion = ecsManager.GetComponent<Expansion>(entity);

		if (transform.scale.x <= expansion.finalScale.x && transform.scale.y <= expansion.finalScale.y)
		{
			ecsManager.AddComponent<EntityDestruction>(entity, EntityDestruction{0});
		}
	}

	return true;
}