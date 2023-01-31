#ifndef __Match3_Scene_Manager_h__
#define __Match3_Scene_Manager_h__

#include <SDL2/SDL.h>

#include "ECS/Manager.hpp"

#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"
#include "Components/SpriteAnimation.hpp"
#include "Components/TileObject.hpp"
#include "Components/Swappable.hpp"
#include "Components/Translate.hpp"
#include "Components/EntityDestruction.hpp"
#include "Components/ClearedTile.hpp"
#include "Components/Expansion.hpp"
#include "Components/Selected.hpp"

#include "Systems/Gravity.hpp"
#include "Systems/Render.hpp"
#include "Systems/ClickTile.hpp"
#include "Systems/MoveTile.hpp"
#include "Systems/DestroyEntity.hpp"
#include "Systems/Expand.hpp"
#include "Systems/ClearTile.hpp"
#include "Systems/AnimateSelected.hpp"
#include "Systems/AnimateSprite.hpp"

extern ECS_Manager ecsManager;

class SceneManager
{
public:
    void Init()
    {
        ecsManager.RegisterComponent<Transform>();
        ecsManager.RegisterComponent<Sprite>();
        ecsManager.RegisterComponent<SpriteAnimation>();
        ecsManager.RegisterComponent<TileObject>();
        ecsManager.RegisterComponent<Swappable>();
        ecsManager.RegisterComponent<Translate>();
        ecsManager.RegisterComponent<Expansion>();
        ecsManager.RegisterComponent<EntityDestruction>();
        ecsManager.RegisterComponent<ClearedTile>();
        ecsManager.RegisterComponent<Selected>();

        renderSystem = ecsManager.RegisterSystem<RenderSystem>();
        clickTileSystem = ecsManager.RegisterSystem<ClickTileSystem>();
        moveTileSystem = ecsManager.RegisterSystem<MoveTileSystem>();
        expandSystem = ecsManager.RegisterSystem<ExpandSystem>();
        destroyEntitySystem = ecsManager.RegisterSystem<DestroyEntitySystem>();
        clearTileSystem = ecsManager.RegisterSystem<ClearTileSystem>();
        animateSelectedSystem = ecsManager.RegisterSystem<AnimateSelectedSystem>();
        animateSpriteSystem = ecsManager.RegisterSystem<AnimateSpriteSystem>();
    };

    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<ClickTileSystem> clickTileSystem;
    std::shared_ptr<MoveTileSystem> moveTileSystem;
    std::shared_ptr<ExpandSystem> expandSystem;
    std::shared_ptr<DestroyEntitySystem> destroyEntitySystem;
    std::shared_ptr<ClearTileSystem> clearTileSystem;
    std::shared_ptr<AnimateSelectedSystem> animateSelectedSystem;
    std::shared_ptr<AnimateSpriteSystem> animateSpriteSystem;
};

#endif //__Match3_Scene_Level_h__