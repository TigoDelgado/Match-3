#ifndef __Match3_Entity_Gem_h__
#define __Match3_Entity_Gem_h__

#include "ECS/Manager.hpp"
#include "ECS/Definitions.hpp"

#include "Components/Position.hpp"
#include "Components/Sprite.hpp"
#include "Components/TileObject.hpp"

#include "RenderWindow.hpp"

extern ECS_Manager ecsManager;

namespace Gem 
{
    Entity CreateEntity(Vector2f p_position, SDL_Texture* p_texture)
    {
        Entity gem = ecsManager.CreateEntity();

	    ecsManager.AddComponent(gem, Position{p_position});

        ecsManager.AddComponent(gem, Sprite{p_texture});

        return gem;
    }
}


#endif //__Match3_Entity_Gem_h__