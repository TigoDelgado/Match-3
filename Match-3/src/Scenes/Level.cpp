#include "Scenes/Level.hpp"

#include "Entities/Gem.hpp"

#include "Components/Position.hpp"
#include "Components/Body.hpp"
#include "Components/Sprite.hpp"

extern ECS_Manager ecsManager;

Level::Level(RenderWindow& p_window, int p_rows, int p_cols, const char* p_background)
    :window(p_window), rows(p_rows), cols(p_cols)
{
    /* ------------------------------ Register Components ------------------------------ */
    
    ecsManager.RegisterComponent<Position>();
    ecsManager.RegisterComponent<Body>();
    ecsManager.RegisterComponent<Sprite>();


    /* -------------------------------- Register Systems -------------------------------- */

    gravitySystem = ecsManager.RegisterSystem<GravitySystem>();
    renderSystem = ecsManager.RegisterSystem<RenderSystem>();


    /* ---------------------------------- Create Scene ---------------------------------- */

    background = window.LoadTexture(p_background);
    
    SDL_Texture* gemBlackTexture = window.LoadTexture("res/Color-1.png");
    SDL_Texture* gemWhiteTexture = window.LoadTexture("res/Color-2.png");
    SDL_Texture* gemPinkTexture = window.LoadTexture("res/Color-3.png");
    SDL_Texture* gemBlueTexture = window.LoadTexture("res/Color-4.png");
    SDL_Texture* gemOrangeTexture = window.LoadTexture("res/Color-5.png");

    Gem::CreateEntity(480.0f, 0.0f, gemBlackTexture);
    Gem::CreateEntity(540.0f, 0.0f, gemWhiteTexture);
    Gem::CreateEntity(600.0f, 0.0f, gemPinkTexture);
    Gem::CreateEntity(660.0f, 0.0f, gemBlueTexture);
    Gem::CreateEntity(720.0f, 0.0f, gemOrangeTexture);
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
    window.Render(background, .0f, .0f, 1280, 720);
    renderSystem->Update(window);
    window.Display();
}