#include "ECS/Manager.hpp"
#include "Systems/Expand.hpp"

#include "Components/Transform.hpp"
#include "Components/Expansion.hpp"

#include "Math2D.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

void ExpandSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
    signature.set(ecsManager.GetComponentType<Expansion>());

	ecsManager.SetSystemSignature<ExpandSystem>(signature);
}


bool ExpandSystem::Update(float dt)
{
	if (entities.size() <= 0) return false;

	for (Entity const& entity : entities)
	{
		Transform& transform = ecsManager.GetComponent<Transform>(entity);
        Expansion& expansion = ecsManager.GetComponent<Expansion>(entity);

		if ((expansion.speed > 0 && transform.scale.x >= expansion.finalScale.x && transform.scale.y >= expansion.finalScale.y) ||
			(expansion.speed < 0 && transform.scale.x <= expansion.finalScale.x && transform.scale.y <= expansion.finalScale.y) )
		{
			ecsManager.RemoveComponent<Expansion>(entity);
		}
		else
		{
			transform.scale = transform.scale + expansion.speed * dt;
            expansion.speed += expansion.acceleration;

			if (expansion.speed > 0)
			{
	            if (transform.scale.x > expansion.finalScale.x)
            	{
                	transform.scale.x = expansion.finalScale.x;
            	}
				if (transform.scale.y > expansion.finalScale.y)
            	{
                	transform.scale.y = expansion.finalScale.y;
            	}				
			}
			else // if (expansion.speed < 0)
			{
				if (transform.scale.x < expansion.finalScale.x)
				{
					transform.scale.x = expansion.finalScale.x;
				}
				if (transform.scale.y < expansion.finalScale.y)
				{
					transform.scale.y = expansion.finalScale.y;
				}
			}
		}
	}

	return true;
}