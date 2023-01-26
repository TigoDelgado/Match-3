#include "EntityCreator.hpp"
#include "GameDefinitions.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

Entity EntityCreator::CreateTileEntity(Vector2f p_position, TileColor p_color, Coordinates p_coords, TileType p_type, TileState p_state)
{
    Entity entity = ecsManager.CreateEntity();

	ecsManager.AddComponent(entity, Transform{p_position, Vector2f{1.0f, 1.0f}, 0.0f});

    ecsManager.AddComponent(entity, Sprite{GetTileTexture(p_color, p_type), Vector2f{TILE_WIDTH, TILE_HEIGHT}});

    ecsManager.AddComponent(entity, TileObject{p_color, p_coords, p_state});

    ecsManager.AddComponent(entity, Swappable{});

    return entity;
}

SDL_Texture* EntityCreator::GetTileTexture(TileColor p_color, TileType p_type)
{
    switch (p_color)     // TODO check isMovable too?
    {
    case TileColor::Black:
        return blackGemTexture;

    case TileColor::White:
        return whiteGemTexture;

    case TileColor::Pink:
        return pinkGemTexture;

    case TileColor::Blue:
        return blueGemTexture;

    case TileColor::Orange:
        return orangeGemTexture;

    case TileColor::Colorless:
        return colorlessGemTexture;
    
    default:
        return NULL;    //FIXME throw exception pls
        break;
    }
}

void EntityCreator::LoadTextures(RenderWindow& window)
{
    blackGemTexture = window.LoadTexture("res/Color-1.png");
    whiteGemTexture = window.LoadTexture("res/Color-2.png");
    pinkGemTexture = window.LoadTexture("res/Color-3.png");
    blueGemTexture = window.LoadTexture("res/Color-4.png");
    orangeGemTexture = window.LoadTexture("res/Color-5.png");
    colorlessGemTexture = window.LoadTexture("res/Color-1.png"); // FIXME get real texture
}