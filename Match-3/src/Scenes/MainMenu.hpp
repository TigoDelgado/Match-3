#ifndef __Match3_Scene_Main_Menu_h__
#define __Match3_Scene_Main_Menu_h__

#include <SDL2/SDL.h>

#include "GameScene.hpp"
#include "RenderWindow.hpp"
#include "GameDefinitions.hpp"

#include "SceneManager.hpp"
#include "AudioManager.hpp"
#include "EntityCreator.hpp"
#include "ECS/Manager.hpp"


class MainMenu : public GameScene
{
public:
    MainMenu(RenderWindow& p_window);

    //Main loop functions
    void HandleEvent(SDL_Event& event) override;
    void Update(float dt) override;
    void Render() override;
    bool ChangeScene() override;
    GameScene* GetNextScene() override;

    ~MainMenu();

private:
    RenderWindow window;
    Vector2f windowDimensions;

    SDL_Texture* background;
    SDL_Texture* startButton;

    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<ClickButtonSystem> clickButtonSystem;
    std::shared_ptr<UpdateTextSystem> updateTextSystem;

    std::vector<Entity> buttons;
    int buttonClicked;
    
    Entity colsText;
    Entity rowsText;
    Entity colorsText;

    bool startLevel = false;

    int cols = 8;
    int rows = 8;
    int colors = 5;
    
    // EntityCreator entityCreator;
};

#endif //__Match3_Scene_Main_Menu_h__