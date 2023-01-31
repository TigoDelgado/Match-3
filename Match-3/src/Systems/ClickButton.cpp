#include "ECS/Manager.hpp"
#include "Systems/ClickButton.hpp"

#include "GameDefinitions.hpp"

#include "Components/Transform.hpp"
#include "Components/Button.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

void ClickButtonSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Transform>());
    signature.set(ecsManager.GetComponentType<Button>());

	ecsManager.SetSystemSignature<ClickButtonSystem>(signature);
}


int ClickButtonSystem::ClickedButton(Vector2f p_point)
{
    for (Entity const& entity : entities)
	{
		Transform& transform = ecsManager.GetComponent<Transform>(entity);
        Button& button = ecsManager.GetComponent<Button>(entity);

        Vector2f offset = {button.dimensions.x / 2, button.dimensions.y / 2};

        if (transform.position.x - offset.x < p_point.x && p_point.x < transform.position.x + offset.x &&
            transform.position.y - offset.y < p_point.y && p_point.y < transform.position.y + offset.y)
        {
            return button.index;
        }
	}
    return -1;
}