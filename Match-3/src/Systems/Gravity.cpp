#include <iostream>

#include "ECS/Manager.hpp"
#include "Systems/Gravity.hpp"

#include "Components/Position.hpp"
#include "Components/Body.hpp"

extern ECS_Manager ecsManager;

void GravitySystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Position>());
	signature.set(ecsManager.GetComponentType<Body>());
	ecsManager.SetSystemSignature<GravitySystem>(signature);
}

void GravitySystem::Update(float dt)
{
	for (auto const& entity : entities)
	{
		std::cout << (entity) << std::endl;
		auto& position = ecsManager.GetComponent<Position>(entity);
		auto& body = ecsManager.GetComponent<Body>(entity);

		position.vector.y += body.gravityForce * dt;
	}
}