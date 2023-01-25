#ifndef __Match3_Scene_Level_h__
#define __Match3_Scene_Level_h__

#include <SDL2/SDL.h>

#include "GameScene.hpp"
#include "RenderWindow.hpp"
#include "Board.hpp"
#include "EntityCreator.hpp"

#include "ECS/Manager.hpp"
#include "Systems/Gravity.hpp"
#include "Systems/Render.hpp"

// GameScene interface
class Level : public GameScene
{
public:
    Level(RenderWindow& p_window, int p_rows, int p_cols, const char* p_background);

    //Main loop functions
    void HandleEvent(SDL_Event& event) override;
    void Update(float dt) override;
    void Render() override;

    virtual ~Level(){};

private:
    RenderWindow window;
    int rows;
    int cols;

    EntityCreator entityCreator;
    SDL_Texture* background;
    Board* board;

    std::shared_ptr<GravitySystem> gravitySystem;
    std::shared_ptr<RenderSystem> renderSystem;
};

#endif //__Match3_Scene_Level_h__