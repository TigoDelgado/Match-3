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

        window.Render(sprite.texture, transform.position, sprite.dimensions * transform.scale);
    }
}