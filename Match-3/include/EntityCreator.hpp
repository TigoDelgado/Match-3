#ifndef __Match3_Entity_Creator_h__
#define __Match3_Entity_Creator_h__

#include "RenderWindow.hpp"
#include "Math2D.hpp"

#include "ECS/Manager.hpp"
#include "ECS/Definitions.hpp"

#include "Components/TileObject.hpp"
#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"
#include "Components/Swappable.hpp"
#include "Components/SpriteAnimation.hpp"
#include "Components/DelayedAnimation.hpp"
#include "Components/Button.hpp"

class EntityCreator
{
public: 
    Entity CreateTileEntity(Vector2f p_position, TileColor p_color, Coordinates p_coords, TileType p_type);
    void UpdateTileSprite(Entity p_entity);

    Entity CreateButtonEntity(int p_index, ButtonType p_type, Vector2f p_dimensions, Vector2f p_position);

    Entity CreateTextEntity(const char* p_initialText, Vector2f p_position, SDL_Color p_color, RenderWindow& p_window);

    void LoadTextures(RenderWindow& window);
    SDL_Texture* GetTileTexture(TileColor p_color, TileType p_type);
    SDL_Texture* GetButtonTexture(ButtonType p_type);


private:
    SDL_Texture* blackTile;
    SDL_Texture* blueTile;
    SDL_Texture* greenTile;
    SDL_Texture* magentaTile;
    SDL_Texture* purpleTile;
    SDL_Texture* whiteTile;
    SDL_Texture* yellowTile;

    SDL_Texture* colorlessTile;

    SDL_Texture* blackVerticalTile;
    SDL_Texture* blueVerticalTile;
    SDL_Texture* greenVerticalTile;
    SDL_Texture* magentaVerticalTile;
    SDL_Texture* purpleVerticalTile;
    SDL_Texture* whiteVerticalTile;
    SDL_Texture* yellowVerticalTile;

    SDL_Texture* blackHorizontalTile;
    SDL_Texture* blueHorizontalTile;
    SDL_Texture* greenHorizontalTile;
    SDL_Texture* magentaHorizontalTile;
    SDL_Texture* purpleHorizontalTile;
    SDL_Texture* whiteHorizontalTile;
    SDL_Texture* yellowHorizontalTile;

    SDL_Texture* blackExplodingTile;
    SDL_Texture* blueExplodingTile;
    SDL_Texture* greenExplodingTile;
    SDL_Texture* magentaExplodingTile;
    SDL_Texture* purpleExplodingTile;
    SDL_Texture* whiteExplodingTile;
    SDL_Texture* yellowExplodingTile;

    SDL_Texture* blackBigExplodingTile;
    SDL_Texture* blueBigExplodingTile;
    SDL_Texture* greenBigExplodingTile;
    SDL_Texture* magentaBigExplodingTile;
    SDL_Texture* purpleBigExplodingTile;
    SDL_Texture* whiteBigExplodingTile;
    SDL_Texture* yellowBigExplodingTile;

    SDL_Texture* blackConsumingTile;
    SDL_Texture* blueConsumingTile;
    SDL_Texture* greenConsumingTile;
    SDL_Texture* magentaConsumingTile;
    SDL_Texture* purpleConsumingTile;
    SDL_Texture* whiteConsumingTile;
    SDL_Texture* yellowConsumingTile;

    SDL_Texture* startButton;
    SDL_Texture* quitButton;
    SDL_Texture* plusButton;
    SDL_Texture* minusButton;
    SDL_Texture* backButton;
    SDL_Texture* defaultButton;
};


#endif //__Match3_Entity_Creator_h__