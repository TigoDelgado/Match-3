#include "ECS/Manager.hpp"
#include "Systems/MoveTile.hpp"

#include "Components/Transform.hpp"
#include "Components/TileObject.hpp"
#include "Components/Translate.hpp"

#include "Math2D.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

void MoveTileSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
    signature.set(ecsManager.GetComponentType<TileObject>());
    signature.set(ecsManager.GetComponentType<Translate>());

	ecsManager.SetSystemSignature<MoveTileSystem>(signature);
}

bool MoveTileSystem::Update(float dt)
{
	if (entities.size() <= 0) return false;

	for (Entity const& entity : entities)
	{
		Transform& transform = ecsManager.GetComponent<Transform>(entity);
        Translate& translate = ecsManager.GetComponent<Translate>(entity);

		if (transform.position == translate.destination)
		{
			ecsManager.RemoveComponent<Translate>(entity);
		}
		else
		{
			if (translate.speed < translate.maxSpeed)				// Has not reach top speed yet
			{
				translate.speed += translate.acceleration * dt;		// Add acceleration force
			}

			Vector2f direction = translate.destination - transform.position;
			direction = direction.normalize();

			Vector2f newPosition = transform.position + (direction * translate.speed * dt);

			if (Arrived(transform.position, translate.destination, newPosition))
			{
				transform.position = translate.destination;
			}
			else 
			{
				transform.position = newPosition;
			}
		}
	}

	return true;
}

// TODO move to Vector2f struct
bool MoveTileSystem::Arrived (Vector2f init, Vector2f dest, Vector2f final)
{
	return (((dest.x - init.x <= 0 && dest.x - final.x >= 0) || (dest.x - init.x >= 0 && dest.x - final.x <= 0)) &&
			((dest.y - init.y <= 0 && dest.y - final.y >= 0) || (dest.y - init.y >= 0 && dest.y - final.y <= 0)));
}