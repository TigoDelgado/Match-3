#include <iostream>

#include "ECS/Manager.hpp"
#include "Systems/Gravity.hpp"

#include "Components/Transform.hpp"

extern ECS_Manager ecsManager;

void GravitySystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
	ecsManager.SetSystemSignature<GravitySystem>(signature);
}

void GravitySystem::Update(float dt)
{
	for (Entity const& entity : entities)
	{
		auto& transform = ecsManager.GetComponent<Transform>(entity);

		transform.position.y += 1.0f * dt;
	}
}