#include "ECS/Manager.hpp"
#include "Systems/AnimateSprite.hpp"

#include "GameDefinitions.hpp"

#include "Components/Sprite.hpp"
#include "Components/SpriteAnimation.hpp"

extern ECS_Manager ecsManager;

void AnimateSpriteSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Sprite>());
    signature.set(ecsManager.GetComponentType<SpriteAnimation>());

	ecsManager.SetSystemSignature<AnimateSpriteSystem>(signature);
}

void AnimateSpriteSystem::Update(float dt)
{
    for (Entity const& entity : entities)
	{
		Sprite& sprite = ecsManager.GetComponent<Sprite>(entity);
		SpriteAnimation& animation = ecsManager.GetComponent<SpriteAnimation>(entity);        

        animation.timeCount += dt;

        if (animation.timeCount >= animation.frameTime)
        {
            if (animation.currentFrame >= animation.lastFrame)
            {
                if (animation.loops == 0)
                {
                    // Remove self if reach end of final loop
                    ecsManager.RemoveComponent<SpriteAnimation>(entity);
                }
                else 
                {
                    animation.currentFrame = animation.firstFrame;
                    if (animation.loops > 0) animation.loops--;
                    animation.timeCount = 0;
                }
            }
            else
            {
                animation.currentFrame++;
                animation.timeCount = 0;
            }

            sprite.currentFrame = animation.currentFrame;
        }
    }
}