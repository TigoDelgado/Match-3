#include "TileCreator.hpp"
#include "Entities/Gem.hpp"
#include "Entities/DefaultEntity.hpp"

Entity TileCreator::CreateTileEntity(Vector2f p_position, TileType p_type)
{
    switch (p_type)
    {
    case TileType::BlackGem:
        return Gem::CreateEntity(p_position, blackGemTexture);
        break;

    case TileType::WhiteGem:
        return Gem::CreateEntity(p_position, whiteGemTexture);
        break;
    
    case TileType::PinkGem:
        return Gem::CreateEntity(p_position, pinkGemTexture);
        break;

    case TileType::BlueGem:
        return Gem::CreateEntity(p_position, blueGemTexture);
        break;

    case TileType::OrangeGem:
        return Gem::CreateEntity(p_position, orangeGemTexture);
        break;
    
    
    default:
        return DefaultEntity::CreateEntity();
    }
}


void TileCreator::LoadTextures(RenderWindow& window)
{
    blackGemTexture = window.LoadTexture("res/Color-1.png");
    whiteGemTexture = window.LoadTexture("res/Color-2.png");
    pinkGemTexture = window.LoadTexture("res/Color-3.png");
    blueGemTexture = window.LoadTexture("res/Color-4.png");
    orangeGemTexture = window.LoadTexture("res/Color-5.png");
}