#include "ECS/Manager.hpp"
#include "Systems/ClickTile.hpp"

#include "GameDefinitions.hpp"

#include "Components/Transform.hpp"
#include "Components/TileObject.hpp"
#include "Components/Swappable.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

void ClickTileSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
    signature.set(ecsManager.GetComponentType<TileObject>());
    signature.set(ecsManager.GetComponentType<Swappable>());

	ecsManager.SetSystemSignature<ClickTileSystem>(signature);
}


bool ClickTileSystem::ClickedTile(Vector2f p_point, Entity& p_entity)
{
    for (Entity const& entity : entities)
	{
		Transform& transform = ecsManager.GetComponent<Transform>(entity);
        // TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

        float offset = (TILE_WIDTH / 2) + (TILE_MARGIN / 2);

        if (transform.position.x - offset < p_point.x && p_point.x < transform.position.x + offset &&
            transform.position.y - offset < p_point.y && p_point.y < transform.position.y + offset)
        {
            p_entity = entity;
            return true;
        }
	}
    return false;
}