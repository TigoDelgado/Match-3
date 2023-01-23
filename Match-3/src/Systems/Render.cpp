#include <iostream>

#include "ECS/Manager.hpp"
#include "Systems/Render.hpp"

#include "Components/Position.hpp"
#include "Components/Sprite.hpp"

extern ECS_Manager ecsManager;

void RenderSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Position>());
	signature.set(ecsManager.GetComponentType<Sprite>());
	ecsManager.SetSystemSignature<RenderSystem>(signature);
}

void RenderSystem::Update(RenderWindow& window)
{
	for (auto const& entity : entities)
	{
		auto& position = ecsManager.GetComponent<Position>(entity);
        auto& sprite = ecsManager.GetComponent<Sprite>(entity);

        window.Render(sprite.texture, position.x, position.y);
    }
}