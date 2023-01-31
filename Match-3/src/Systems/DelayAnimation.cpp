#include "ECS/Manager.hpp"
#include "Systems/DelayAnimation.hpp"

#include "GameDefinitions.hpp"

#include "Components/SpriteAnimation.hpp"
#include "Components/DelayedAnimation.hpp"

extern ECS_Manager ecsManager;

void DelayAnimationSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<DelayedAnimation>());

	ecsManager.SetSystemSignature<DelayAnimationSystem>(signature);
}

void DelayAnimationSystem::Update(float dt)
{
    for (Entity const& entity : entities)
	{
		DelayedAnimation& animation = ecsManager.GetComponent<DelayedAnimation>(entity);        

        animation.timeCount += dt;

        if (animation.timeCount >= animation.delay)             // wait for <delay> seconds
        {
            ecsManager.AddComponent<SpriteAnimation>(entity, animation.animation);
            if (animation.loops > 0) animation.loops--;

            if (animation.loops == 0)
            {
                // Remove delayed animation component if no more loops
                ecsManager.RemoveComponent<DelayedAnimation>(entity);
            }

            animation.timeCount = 0.0f;
        }
    }
}