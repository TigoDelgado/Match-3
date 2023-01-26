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


bool ClickTileSystem::ClickedEntity(Vector2f p_point, Coordinates& p_coords)
{
    for (Entity const& entity : entities)
	{
		Transform& transform = ecsManager.GetComponent<Transform>(entity);
        TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

        if (transform.position.x < p_point.x && p_point.x < transform.position.x + TILE_WIDTH &&
            transform.position.y < p_point.y && p_point.y < transform.position.y + TILE_HEIGHT)
        {
            p_coords = tileObject.coords;
            return true;
        }
	}
    return false;
}