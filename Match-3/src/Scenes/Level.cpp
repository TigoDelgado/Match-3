#include "Scenes/Level.hpp"

#include "TileCreator.hpp"

#include "Components/Position.hpp"
#include "Components/Body.hpp"
#include "Components/Sprite.hpp"

extern ECS_Manager ecsManager;

Level::Level(RenderWindow& p_window, int p_rows, int p_cols, const char* p_background)
    :window(p_window), rows(p_rows), cols(p_cols)
{

    

    /* ------------------------------- Setup Resources  ------------------------------- */

    background = window.LoadTexture(p_background);

    // Load Tile object textures
    tileCreator.LoadTextures(window);

    /* ------------------------------ Register Components ------------------------------ */
    
    ecsManager.RegisterComponent<Position>();
    ecsManager.RegisterComponent<Body>();
    ecsManager.RegisterComponent<Sprite>();


    /* -------------------------------- Register Systems -------------------------------- */

    gravitySystem = ecsManager.RegisterSystem<GravitySystem>();
    renderSystem = ecsManager.RegisterSystem<RenderSystem>();



    /* ------------------------------ Create Scene Objects ------------------------------ */

    // TODO pass this through a level manager?
    std::vector<TileType> tileTypes
    {
        TileType::BlackGem,
        TileType::WhiteGem, 
        TileType::PinkGem, 
        TileType::BlueGem, 
        TileType::OrangeGem
    };
    
    board = new Board(Vector2f{400.0f, 100.0f}, p_rows, p_cols, tileTypes);
    board->PopulateBoard(tileCreator);

}

void Level::HandleEvent(SDL_Event& event)
{

}

void Level::Update(float dt)
{
    gravitySystem->Update(dt);
}

void Level::Render()
{
    window.Clear();
    window.Render(background, Vector2f{0.0f, 0.0f}, Vector2f{1280.0f,720.0f});
    renderSystem->Update(window);
    window.Display();
}