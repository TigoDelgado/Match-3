#include <iostream>

#include "Systems/Gravity.hpp"
#include "Components/Position.hpp"
#include "ECS/Manager.hpp"

extern ECS_Manager ecsManager;

void GravitySystem::Init()
{
}

void GravitySystem::Update(float dt)
{
	for (auto const& entity : entities)
	{
		auto& position = ecsManager.GetComponent<Position>(entity);

		position.y -= gravityForce * dt;

		std::cout << "position of " << (entity) << " updated by gravity. x = " << (position.x) << " | y = " << (position.y) << std::endl;
	}
}