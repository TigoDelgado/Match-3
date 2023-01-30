#include "EntityCreator.hpp"
#include "GameDefinitions.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

Entity EntityCreator::CreateTileEntity(Vector2f p_position, TileColor p_color, Coordinates p_coords, TileType p_type)
{
    Entity entity = ecsManager.CreateEntity();

	ecsManager.AddComponent(entity, Transform{p_position, 1.0f, 0.0f});

    ecsManager.AddComponent(entity, TileObject{p_color, p_type, p_coords});

    ecsManager.AddComponent(entity, Sprite{GetTileTexture(p_color, p_type), 0, Vector2f{70, 70}, Vector2f{TILE_WIDTH, TILE_HEIGHT}});

    ecsManager.AddComponent(entity, Swappable{});

    return entity;
}

void EntityCreator::UpdateTileSprite(Entity p_entity)
{
    TileObject& tileObject = ecsManager.GetComponent<TileObject>(p_entity);
    Sprite& sprite = ecsManager.GetComponent<Sprite>(p_entity);

    sprite.texture = GetTileTexture(tileObject.color, tileObject.type);

    // std::cout << "Updating tile of entity " << p_entity << " at [" << tileObject.coords.x << ", " << tileObject.coords.y << "]" << std::endl;
}

SDL_Texture* EntityCreator::GetTileTexture(TileColor p_color, TileType p_type)
{
    if (p_color == TileColor::Black)
    {
        if (p_type == TileType::Normal) return blackTile;
        if (p_type == TileType::Horizontal) return blackHorizontalTile;
        if (p_type == TileType::Vertical) return blackVerticalTile;
        if (p_type == TileType::Exploding) return blackExplodingTile;
        if (p_type == TileType::BigExploding) return blackBigExplodingTile;
        if (p_type == TileType::Consuming) return blackConsumingTile;
    }
    else if (p_color == TileColor::White)
    {
        if (p_type == TileType::Normal) return whiteTile;
        if (p_type == TileType::Horizontal) return whiteHorizontalTile;
        if (p_type == TileType::Vertical) return whiteVerticalTile;
        if (p_type == TileType::Exploding) return whiteExplodingTile;
        if (p_type == TileType::BigExploding) return whiteBigExplodingTile;
        if (p_type == TileType::Consuming) return whiteConsumingTile;
    }
    else if (p_color == TileColor::Pink)
    {
        if (p_type == TileType::Normal) return pinkTile;
        if (p_type == TileType::Horizontal) return pinkHorizontalTile;
        if (p_type == TileType::Vertical) return pinkVerticalTile;
        if (p_type == TileType::Exploding) return pinkExplodingTile;
        if (p_type == TileType::BigExploding) return pinkBigExplodingTile;
        if (p_type == TileType::Consuming) return pinkConsumingTile;
    }
    else if (p_color == TileColor::Blue)
    {
        if (p_type == TileType::Normal) return blueTile;
        if (p_type == TileType::Horizontal) return blueHorizontalTile;
        if (p_type == TileType::Vertical) return blueVerticalTile;
        if (p_type == TileType::Exploding) return blueExplodingTile;
        if (p_type == TileType::BigExploding) return blueBigExplodingTile;
        if (p_type == TileType::Consuming) return blueConsumingTile;
    }
    else if (p_color == TileColor::Orange)
    {
        if (p_type == TileType::Normal) return orangeTile;
        if (p_type == TileType::Horizontal) return orangeHorizontalTile;
        if (p_type == TileType::Vertical) return orangeVerticalTile;
        if (p_type == TileType::Exploding) return orangeExplodingTile;
        if (p_type == TileType::BigExploding) return orangeBigExplodingTile;
        if (p_type == TileType::Consuming) return orangeConsumingTile;
    }
    return colorlessTile;
}

void EntityCreator::LoadTextures(RenderWindow& window)
{
    blackTile = window.LoadTexture("../res/Tiles/Color-1.png");
    whiteTile = window.LoadTexture("../res/Tiles/Color-2.png");
    pinkTile = window.LoadTexture("../res/Tiles/Color-3.png");
    blueTile = window.LoadTexture("../res/Tiles/Color-4.png");
    orangeTile = window.LoadTexture("../res/Tiles/Color-5-sprite-sheet.png");
    colorlessTile = window.LoadTexture("../res/Tiles/Color-1.png"); // FIXME get real texture;

    blackHorizontalTile = window.LoadTexture("../res/Tiles/Color-1-H.png");
    whiteHorizontalTile = window.LoadTexture("../res/Tiles/Color-2-H.png");
    pinkHorizontalTile = window.LoadTexture("../res/Tiles/Color-3-H.png");
    blueHorizontalTile = window.LoadTexture("../res/Tiles/Color-4-H.png");
    orangeHorizontalTile = window.LoadTexture("../res/Tiles/Color-5-H.png");

    blackVerticalTile = window.LoadTexture("../res/Tiles/Color-1-V.png");
    whiteVerticalTile = window.LoadTexture("../res/Tiles/Color-2-V.png");
    pinkVerticalTile = window.LoadTexture("../res/Tiles/Color-3-V.png");
    blueVerticalTile = window.LoadTexture("../res/Tiles/Color-4-V.png");
    orangeVerticalTile = window.LoadTexture("../res/Tiles/Color-5-V.png");

    blackExplodingTile = window.LoadTexture("../res/Tiles/Color-1-X.png");
    whiteExplodingTile = window.LoadTexture("../res/Tiles/Color-2-X.png");
    pinkExplodingTile = window.LoadTexture("../res/Tiles/Color-3-X.png");
    blueExplodingTile = window.LoadTexture("../res/Tiles/Color-4-X.png");
    orangeExplodingTile = window.LoadTexture("../res/Tiles/Color-5-X.png");

    blackBigExplodingTile = window.LoadTexture("../res/Tiles/Color-1-X2.png");
    whiteBigExplodingTile = window.LoadTexture("../res/Tiles/Color-2-X2.png");
    pinkBigExplodingTile = window.LoadTexture("../res/Tiles/Color-3-X2.png");
    blueBigExplodingTile = window.LoadTexture("../res/Tiles/Color-4-X2.png");
    orangeBigExplodingTile = window.LoadTexture("../res/Tiles/Color-5-X2.png");

    blackConsumingTile = window.LoadTexture("../res/Tiles/Color-1-C.png");
    whiteConsumingTile = window.LoadTexture("../res/Tiles/Color-2-C.png");
    pinkConsumingTile = window.LoadTexture("../res/Tiles/Color-3-C.png");
    blueConsumingTile = window.LoadTexture("../res/Tiles/Color-4-C.png");
    orangeConsumingTile = window.LoadTexture("../res/Tiles/Color-5-C.png");
}