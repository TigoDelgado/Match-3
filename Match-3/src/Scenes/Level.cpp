#include "Scenes/Level.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

Level::Level(RenderWindow& p_window, int p_rows, int p_cols, const char* p_background)
    :window(p_window), rows(p_rows), cols(p_cols)
{

    

    /* ------------------------------- Setup Resources  ------------------------------- */

    background = window.LoadTexture(p_background);

    // Load Tile object textures
    entityCreator.LoadTextures(window);

    /* ------------------------------ Register Components ------------------------------ */
    
    ecsManager.RegisterComponent<Transform>();
    ecsManager.RegisterComponent<Sprite>();
    ecsManager.RegisterComponent<TileObject>();
    ecsManager.RegisterComponent<Swappable>();


    /* -------------------------------- Register Systems -------------------------------- */

    // gravitySystem = ecsManager.RegisterSystem<GravitySystem>();
    renderSystem = ecsManager.RegisterSystem<RenderSystem>();
    clickTileSystem = ecsManager.RegisterSystem<ClickTileSystem>();
    // FIXME Don't allow systems to register without required components


    /* ------------------------------ Create Scene Objects ------------------------------ */

    // TODO pass this through a level manager?
    std::vector<TileObject> tileObjects
    {
        {TileColor::Black, true},
        {TileColor::White, true},
        {TileColor::Pink, true},
        {TileColor::Blue, true},
        {TileColor::Orange, true}
    };
    
    board = new Board(Vector2f{400.0f, 200.0f}, p_rows, p_cols, entityCreator);
    board->PopulateBoard(tileObjects);

}

void Level::HandleEvent(SDL_Event& event)
{
    switch(event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                //handle a left-click
                Entity entity;
                if ( clickTileSystem->ClickedEntity(Vector2f{float(event.button.x), float(event.button.y)}, entity) )
                    std::cout << "Clicked on entity: " << (entity) << std::endl;
                else std::cout << "Clicked on NOTHING!" << std::endl;
            }
            break;
        default:
            break;
        }
}

void Level::Update(float dt)
{
    // gravitySystem->Update(dt);
}

void Level::Render()
{
    window.Clear();
    window.Render(background, Vector2f{0.0f, 0.0f}, Vector2f{1280.0f,720.0f});
    renderSystem->Update(window);
    window.Display();
}