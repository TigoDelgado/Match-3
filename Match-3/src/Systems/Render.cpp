#include <iostream>

#include "ECS/Manager.hpp"
#include "Systems/Render.hpp"

#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"

#include "Math2D.hpp"

extern ECS_Manager ecsManager;

void RenderSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
	signature.set(ecsManager.GetComponentType<Sprite>());
	ecsManager.SetSystemSignature<RenderSystem>(signature);
}

void RenderSystem::Update(RenderWindow& window)
{
	for (auto const& entity : entities)
	{
		auto& transform = ecsManager.GetComponent<Transform>(entity);
        auto& sprite = ecsManager.GetComponent<Sprite>(entity);

		// Images are rendered starting from the top-left point
		Vector2f topLeft = transform.position - (sprite.destDimensions * transform.scale) / 2;
		Vector2f sheetOffset {sprite.currentFrame * sprite.sourceDimensions.x, 0};
		Vector2f sourceDimensions = sprite.sourceDimensions;
		Vector2f destDimensions = sprite.destDimensions * transform.scale;

        window.Render(sprite.texture, topLeft, sheetOffset, sourceDimensions, destDimensions, transform.rotation, NULL, SDL_FLIP_NONE);
    }
}