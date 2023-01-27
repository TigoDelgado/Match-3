#include "ECS/Manager.hpp"
#include "Systems/AnimateSelected.hpp"

#include "GameDefinitions.hpp"

#include "Components/Transform.hpp"
#include "Components/TileObject.hpp"
#include "Components/Selected.hpp"

extern ECS_Manager ecsManager;

void AnimateSelectedSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
    signature.set(ecsManager.GetComponentType<TileObject>());
    signature.set(ecsManager.GetComponentType<Selected>());

	ecsManager.SetSystemSignature<AnimateSelectedSystem>(signature);
}

void AnimateSelectedSystem::Update(float dt)
{
    for (Entity const& entity : entities)
	{
		Transform& transform = ecsManager.GetComponent<Transform>(entity);

        transform.rotation += TILE_ROTATE_SPEED * dt;
    }
}