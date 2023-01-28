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

class EntityCreator
{
public: 
    Entity CreateTileEntity(Vector2f p_position, TileColor p_color, Coordinates p_coords, TileType p_type);
    void UpdateTileSprite(Entity p_entity);
    void LoadTextures(RenderWindow& window);
    SDL_Texture* GetTileTexture(TileColor p_color, TileType p_type);

private:
    SDL_Texture* blackTile;
    SDL_Texture* whiteTile;
    SDL_Texture* pinkTile;
    SDL_Texture* blueTile;
    SDL_Texture* orangeTile;
    SDL_Texture* colorlessTile;

    SDL_Texture* blackHorizontalTile;
    SDL_Texture* whiteHorizontalTile;
    SDL_Texture* pinkHorizontalTile;
    SDL_Texture* blueHorizontalTile;
    SDL_Texture* orangeHorizontalTile;

    SDL_Texture* blackVerticalTile;
    SDL_Texture* whiteVerticalTile;
    SDL_Texture* pinkVerticalTile;
    SDL_Texture* blueVerticalTile;
    SDL_Texture* orangeVerticalTile;

    SDL_Texture* blackExplodingTile;
    SDL_Texture* whiteExplodingTile;
    SDL_Texture* pinkExplodingTile;
    SDL_Texture* blueExplodingTile;
    SDL_Texture* orangeExplodingTile;

    SDL_Texture* blackConsumingTile;
    SDL_Texture* whiteConsumingTile;
    SDL_Texture* pinkConsumingTile;
    SDL_Texture* blueConsumingTile;
    SDL_Texture* orangeConsumingTile;
};


#endif //__Match3_Entity_Creator_h__