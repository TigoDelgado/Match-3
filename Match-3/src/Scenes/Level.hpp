#ifndef __Match3_Scene_Level_h__
#define __Match3_Scene_Level_h__

#include <SDL2/SDL.h>

#include "GameScene.hpp"
#include "RenderWindow.hpp"
#include "Board.hpp"
#include "EntityCreator.hpp"

#include "ECS/Manager.hpp"
#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"
#include "Components/SpriteAnimation.hpp"
#include "Components/TileObject.hpp"
#include "Components/Swappable.hpp"
#include "Components/Translate.hpp"
#include "Components/EntityDestruction.hpp"
#include "Components/ClearedTile.hpp"
#include "Components/Shrinking.hpp"
#include "Components/Selected.hpp"

#include "ECS/Manager.hpp"
#include "Systems/Gravity.hpp"
#include "Systems/Render.hpp"
#include "Systems/ClickTile.hpp"
#include "Systems/MoveTile.hpp"
#include "Systems/DestroyEntity.hpp"
#include "Systems/ShrinkEntity.hpp"
#include "Systems/ClearTile.hpp"
#include "Systems/AnimateSelected.hpp"
#include "Systems/AnimateSprite.hpp"


enum LevelState
{
    INITIALIZING,
    WAITING,
    SWAPPING_TILES,
    SWAPPING_BACK,
    CLEARING_MATCHES,
    ACTIVATING_SPECIAL,
    CREATING_SPECIAL,
    SPAWNING_TILES,
    GRAVITATING_ROWS,
    CLEARING_LOOP,
    GAME_OVER
};


class Level : public GameScene
{
public:
    Level(RenderWindow& p_window, int p_rows, int p_cols, const char* p_background, const char* p_boardTexture);

    //Main loop functions
    void HandleEvent(SDL_Event& event) override;
    void Update(float dt) override;
    void Render() override;

    virtual ~Level(){};

private:
    RenderWindow window;
    Vector2f windowDimensions;
    int rows;
    int cols;

    EntityCreator entityCreator;
    SDL_Texture* background;
    SDL_Texture* boardTexture;
    SDL_Texture* scoreText;
    Board* board;
    std::vector<TileColor> tileColors;

    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<ClickTileSystem> clickTileSystem;
    std::shared_ptr<MoveTileSystem> moveTileSystem;
    std::shared_ptr<ShrinkEntitySystem> shrinkEntitySystem;
    std::shared_ptr<DestroyEntitySystem> destroyEntitySystem;
    std::shared_ptr<ClearTileSystem> clearTileSystem;
    std::shared_ptr<AnimateSelectedSystem> animateSelectedSystem;
    std::shared_ptr<AnimateSpriteSystem> animateSpriteSystem;

    LevelState state = INITIALIZING;

    bool blocked = true;
    bool changedSelectedOne = false;
    bool changedSelectedTwo = false;
    Entity lastSelected = NULL_ENTITY;
    Entity selectedOne = NULL_ENTITY;
    Entity selectedTwo = NULL_ENTITY;
    bool mouseDown = false;
};

#endif //__Match3_Scene_Level_h__