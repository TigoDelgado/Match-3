#include "EntityCreator.hpp"
#include "GameDefinitions.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

Entity EntityCreator::CreateTileEntity(Vector2f p_position, TileColor p_color, Coordinates p_coords, TileType p_type)
{
    Entity entity = ecsManager.CreateEntity();

	ecsManager.AddComponent(entity, Transform{p_position, Vector2f{1.0f, 1.0f}, 0.0f});

    ecsManager.AddComponent(entity, TileObject{p_color, p_type, p_coords});

    ecsManager.AddComponent(entity, Sprite{GetTileTexture(p_color, p_type), 0, Vector2f{TILE_SOURCE_WIDTH, TILE_SOURCE_HEIGHT}, Vector2f{TILE_WIDTH, TILE_HEIGHT}});

    ecsManager.AddComponent(entity, Swappable{});

    if (p_type == TileType::Consuming)
    {
        SpriteAnimation animation{0, 4, 0, 0, 0, 0.1f, 0.0f};
        ecsManager.AddComponent(entity, DelayedAnimation{animation, 5.0f, 3.0f, -1});
    }

    return entity;
}

Entity EntityCreator::CreateButtonEntity(int p_index, ButtonType p_type, Vector2f p_dimensions, Vector2f p_position)
{
    Entity entity = ecsManager.CreateEntity();

    ecsManager.AddComponent(entity, Transform{p_position, Vector2f{1.0f, 1.0f}, 0.0f});

    SDL_Texture* texture = GetButtonTexture(p_type);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    ecsManager.AddComponent(entity, Sprite{texture, 0, Vector2f{float(width), float(height)}, p_dimensions});

    ecsManager.AddComponent(entity, Button{p_index, p_type, p_dimensions});

    return entity;
}

Entity EntityCreator::CreateTextEntity(const char* p_initialText, Vector2f p_position, SDL_Color p_color, RenderWindow& p_window)
{
    Entity entity = ecsManager.CreateEntity();

    ecsManager.AddComponent(entity, Transform{p_position, Vector2f{1.0f, 1.0f}, 0.0f});

    SDL_Texture* texture = p_window.LoadTextureFromText(p_initialText, p_color);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    ecsManager.AddComponent(entity, Sprite{texture, 0, Vector2f{float(width), float(height)}, Vector2f{float(width), float(height)}});

    return entity;
}

void EntityCreator::UpdateTileSprite(Entity p_entity)
{
    TileObject& tileObject = ecsManager.GetComponent<TileObject>(p_entity);
    Sprite& sprite = ecsManager.GetComponent<Sprite>(p_entity);

    sprite.texture = GetTileTexture(tileObject.color, tileObject.type);

    // TODO create animation manager to take care of this
    // Quick and dirty way to add iddle animation to special tiles -- they never transform back into normal tiles
    if (tileObject.type != TileType::Normal)
    {
        SpriteAnimation animation;
        if (tileObject.type == TileType::BigExploding || tileObject.type == TileType::Consuming) animation = {0, 4, 0, 0, 1, 0.05f, 0.0f};
        else animation = {0, 4, 0, 0, 0, 0.05f, 0.0f};
        ecsManager.AddComponent(p_entity, DelayedAnimation{animation, 5.0f, 3.0f, -1});
    }
}

SDL_Texture* EntityCreator::GetButtonTexture(ButtonType p_type)
{
    switch (p_type)
    {
    case ButtonType::START:
        return startButton;
        break;

    case ButtonType::QUIT:
        return quitButton;
        break;

    case ButtonType::PLUS:
        return plusButton;
        break;

    case ButtonType::MINUS:
        return minusButton;
        break;
    
    default:
        return defaultButton;
        break;
    }
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
    else if (p_color == TileColor::Blue)
    {
        if (p_type == TileType::Normal) return blueTile;
        if (p_type == TileType::Horizontal) return blueHorizontalTile;
        if (p_type == TileType::Vertical) return blueVerticalTile;
        if (p_type == TileType::Exploding) return blueExplodingTile;
        if (p_type == TileType::BigExploding) return blueBigExplodingTile;
        if (p_type == TileType::Consuming) return blueConsumingTile;
    }
    else if (p_color == TileColor::Green)
    {
        if (p_type == TileType::Normal) return greenTile;
        if (p_type == TileType::Horizontal) return greenHorizontalTile;
        if (p_type == TileType::Vertical) return greenVerticalTile;
        if (p_type == TileType::Exploding) return greenExplodingTile;
        if (p_type == TileType::BigExploding) return greenBigExplodingTile;
        if (p_type == TileType::Consuming) return greenConsumingTile;
    }
    else if (p_color == TileColor::Magenta)
    {
        if (p_type == TileType::Normal) return magentaTile;
        if (p_type == TileType::Horizontal) return magentaHorizontalTile;
        if (p_type == TileType::Vertical) return magentaVerticalTile;
        if (p_type == TileType::Exploding) return magentaExplodingTile;
        if (p_type == TileType::BigExploding) return magentaBigExplodingTile;
        if (p_type == TileType::Consuming) return magentaConsumingTile;
    }
    else if (p_color == TileColor::Purple)
    {
        if (p_type == TileType::Normal) return purpleTile;
        if (p_type == TileType::Horizontal) return purpleHorizontalTile;
        if (p_type == TileType::Vertical) return purpleVerticalTile;
        if (p_type == TileType::Exploding) return purpleExplodingTile;
        if (p_type == TileType::BigExploding) return purpleBigExplodingTile;
        if (p_type == TileType::Consuming) return purpleConsumingTile;
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
    else if (p_color == TileColor::Yellow)
    {
        if (p_type == TileType::Normal) return yellowTile;
        if (p_type == TileType::Horizontal) return yellowHorizontalTile;
        if (p_type == TileType::Vertical) return yellowVerticalTile;
        if (p_type == TileType::Exploding) return yellowExplodingTile;
        if (p_type == TileType::BigExploding) return yellowBigExplodingTile;
        if (p_type == TileType::Consuming) return yellowConsumingTile;
    }
    return colorlessTile;
}

void EntityCreator::LoadTextures(RenderWindow& window)
{
    blackTile = window.LoadTexture("../res/Tiles/Black.png");
    blueTile = window.LoadTexture("../res/Tiles/Blue.png");
    greenTile = window.LoadTexture("../res/Tiles/DGreen.png");
    magentaTile = window.LoadTexture("../res/Tiles/Magenta.png");
    purpleTile = window.LoadTexture("../res/Tiles/Purple.png");
    whiteTile = window.LoadTexture("../res/Tiles/White.png");
    // yellowTile = window.LoadTexture("../res/Tiles/Yellow.png");
    yellowTile = window.LoadTexture("../res/Tiles/Gold.png");

    colorlessTile = window.LoadTexture("../res/Tiles/Colorless.png");

    blackVerticalTile = window.LoadTexture("../res/Tiles/Black-V.png");
    blueVerticalTile = window.LoadTexture("../res/Tiles/Blue-V.png");
    greenVerticalTile = window.LoadTexture("../res/Tiles/DGreen-V.png");
    magentaVerticalTile = window.LoadTexture("../res/Tiles/Magenta-V.png");
    purpleVerticalTile = window.LoadTexture("../res/Tiles/Purple-V.png");
    whiteVerticalTile = window.LoadTexture("../res/Tiles/White-V.png");
    // yellowVerticalTile = window.LoadTexture("../res/Tiles/Yellow-V.png");
    yellowVerticalTile = window.LoadTexture("../res/Tiles/Gold-V.png");

    blackHorizontalTile = window.LoadTexture("../res/Tiles/Black-H.png");
    blueHorizontalTile = window.LoadTexture("../res/Tiles/Blue-H.png");
    greenHorizontalTile = window.LoadTexture("../res/Tiles/DGreen-H.png");
    magentaHorizontalTile = window.LoadTexture("../res/Tiles/Magenta-H.png");
    purpleHorizontalTile = window.LoadTexture("../res/Tiles/Purple-H.png");
    whiteHorizontalTile = window.LoadTexture("../res/Tiles/White-H.png");
    // yellowHorizontalTile = window.LoadTexture("../res/Tiles/Yellow-H.png");
    yellowHorizontalTile = window.LoadTexture("../res/Tiles/Gold-H.png");

    blackExplodingTile = window.LoadTexture("../res/Tiles/Black-X.png");
    blueExplodingTile = window.LoadTexture("../res/Tiles/Blue-X.png");
    greenExplodingTile = window.LoadTexture("../res/Tiles/DGreen-X.png");
    magentaExplodingTile = window.LoadTexture("../res/Tiles/Magenta-X.png");
    purpleExplodingTile = window.LoadTexture("../res/Tiles/Purple-X.png");
    whiteExplodingTile = window.LoadTexture("../res/Tiles/White-X.png");
    // yellowExplodingTile = window.LoadTexture("../res/Tiles/Yellow-X.png");
    yellowExplodingTile = window.LoadTexture("../res/Tiles/Gold-X.png");

    blackBigExplodingTile = window.LoadTexture("../res/Tiles/Black-X2.png");
    blueBigExplodingTile = window.LoadTexture("../res/Tiles/Blue-X2.png");
    greenBigExplodingTile = window.LoadTexture("../res/Tiles/DGreen-X2.png");
    magentaBigExplodingTile = window.LoadTexture("../res/Tiles/Magenta-X2.png");
    purpleBigExplodingTile = window.LoadTexture("../res/Tiles/Purple-X2.png");
    whiteBigExplodingTile = window.LoadTexture("../res/Tiles/White-X2.png");
    // yellowBigExplodingTile = window.LoadTexture("../res/Tiles/Yellow-X2.png");
    yellowBigExplodingTile = window.LoadTexture("../res/Tiles/Gold-X2.png");

    blackConsumingTile = window.LoadTexture("../res/Tiles/Black-C2.png");
    blueConsumingTile = window.LoadTexture("../res/Tiles/Blue-C2.png");
    greenConsumingTile = window.LoadTexture("../res/Tiles/DGreen-C2.png");
    magentaConsumingTile = window.LoadTexture("../res/Tiles/Magenta-C2.png");
    purpleConsumingTile = window.LoadTexture("../res/Tiles/Purple-C2.png");
    whiteConsumingTile = window.LoadTexture("../res/Tiles/White-C2.png");
    // yellowConsumingTile = window.LoadTexture("../res/Tiles/Yellow-C2.png");
    yellowConsumingTile = window.LoadTexture("../res/Tiles/Gold-C2.png");

    startButton = window.LoadTexture("../res/start.png");
    quitButton = whiteBigExplodingTile; //window.LoadTexture(QUIT_BUTTON);
    plusButton = window.LoadTexture("../res/plus.png");
    minusButton = window.LoadTexture("../res/minus.png");
    defaultButton = colorlessTile;
}