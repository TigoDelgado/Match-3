#include "ECS/Manager.hpp"
#include "Systems/ShrinkEntity.hpp"

#include "Components/Transform.hpp"
#include "Components/Shrinking.hpp"

#include "Math2D.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

void ShrinkEntitySystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
    signature.set(ecsManager.GetComponentType<Shrinking>());

	ecsManager.SetSystemSignature<ShrinkEntitySystem>(signature);
}


bool ShrinkEntitySystem::Update(float dt)
{
	if (entities.size() <= 0) return false;

	for (Entity const& entity : entities)
	{
		Transform& transform = ecsManager.GetComponent<Transform>(entity);
        Shrinking& shrinking = ecsManager.GetComponent<Shrinking>(entity);

		if (transform.scale.x <= shrinking.finalScale.x && transform.scale.y <= shrinking.finalScale.y)
		{
			ecsManager.RemoveComponent<Shrinking>(entity);
		}
		else
		{
			transform.scale = transform.scale - shrinking.speed * dt;
            shrinking.speed += shrinking.acceleration;

            if (transform.scale.x < shrinking.finalScale.x)
            {
                transform.scale.x = shrinking.finalScale.x;
            }
			if (transform.scale.y < shrinking.finalScale.y)
            {
                transform.scale.y = shrinking.finalScale.y;
            }
		}
	}

	return true;
}