#include <iostream>

#include "ECS/Manager.hpp"
#include "Systems/UpdateText.hpp"

#include "Components/Text.hpp"
#include "Components/Sprite.hpp"

#include "Math2D.hpp"

extern ECS_Manager ecsManager;

void UpdateTextSystem::Init()
{
	Signature signature;
	signature.set(ecsManager.GetComponentType<Text>());
	signature.set(ecsManager.GetComponentType<Sprite>());
    ecsManager.SetSystemSignature<UpdateTextSystem>(signature);
}

void UpdateTextSystem::Update(RenderWindow& window)
{
	for (auto const& entity : entities)
	{
        auto& text = ecsManager.GetComponent<Text>(entity);
        auto& sprite = ecsManager.GetComponent<Sprite>(entity);

        SDL_Texture* texture = window.LoadTextureFromText(text.text, text.color);
        int width, height;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        SDL_DestroyTexture(sprite.texture);
        sprite.texture = texture;
        sprite.sourceDimensions = Vector2f{float(width), float(height)};
        sprite.destDimensions = Vector2f{float(width), float(height)};

        ecsManager.RemoveComponent<Text>(entity);
    }
}