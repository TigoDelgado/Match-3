#ifndef __Match3_Scene_Level_h__
#define __Match3_Scene_Level_h__

#include <SDL2/SDL.h>

#include "GameScene.hpp"
#include "RenderWindow.hpp"
#include "Board.hpp"
#include "EntityCreator.hpp"
#include "Scenes/MainMenu.hpp"

#include "SceneManager.hpp"
#include "AudioManager.hpp"
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
#include "Components/DelayedAnimation.hpp"

#include "Systems/Gravity.hpp"
#include "Systems/Render.hpp"
#include "Systems/ClickTile.hpp"
#include "Systems/MoveTile.hpp"
#include "Systems/DestroyEntity.hpp"
#include "Systems/Expand.hpp"
#include "Systems/ClearTile.hpp"
#include "Systems/AnimateSelected.hpp"
#include "Systems/AnimateSprite.hpp"
#include "Systems/DelayAnimation.hpp"
#include "Systems/ClickButton.hpp"


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


/**
 * A basic match-3 level, containing one board with a grid of tiles 
 * Handles the gameplay loop according to a MCV model
 * 
*/
class Level : public GameScene
{
public:
    Level(RenderWindow& p_window, int p_rows, int p_cols, int p_colors, const char* p_background, const char* p_boardTexture);

    //Main loop functions
    void HandleEvent(SDL_Event& event) override;
    void Update(float dt) override;
    void Render() override;
    bool ChangeScene() override;
    GameScene* GetNextScene() override;

    ~Level();

private:
    RenderWindow window;
    Vector2f windowDimensions;
    int rows;
    int cols;

    Board* board;
    int score = 0;
    int addScore = 0;
    std::vector<TileColor> tileColors;

    SDL_Texture* background;
    SDL_Texture* boardTexture;
    
    LevelState state = INITIALIZING;

    void UpdateSelected();

    /* ---------------------------- Scene Entities ---------------------------- */

    Entity lastSelected = NULL_ENTITY;
    Entity selectedOne = NULL_ENTITY;
    Entity selectedTwo = NULL_ENTITY;
    Entity backButton;
    Entity scoreText;


    /* ----------------------------- Scene Flags ----------------------------- */

    bool blocked = true;
    bool changedSelectedOne = false;
    int buttonClicked = -1;
    bool changedSelectedTwo = false;
    bool backToMenu = false;
    bool mouseDown = false;


    /* ---------------------------- Scene Systems ---------------------------- */

    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<ClickTileSystem> clickTileSystem;
    std::shared_ptr<MoveTileSystem> moveTileSystem;
    std::shared_ptr<ExpandSystem> expandSystem;
    std::shared_ptr<DestroyEntitySystem> destroyEntitySystem;
    std::shared_ptr<ClearTileSystem> clearTileSystem;
    std::shared_ptr<AnimateSelectedSystem> animateSelectedSystem;
    std::shared_ptr<AnimateSpriteSystem> animateSpriteSystem;
    std::shared_ptr<DelayAnimationSystem> delayAnimationSystem;
    std::shared_ptr<UpdateTextSystem> updateTextSystem;
    std::shared_ptr<ClickButtonSystem> clickButtonSystem;
};

#endif //__Match3_Scene_Level_h__