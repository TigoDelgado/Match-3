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
    Entity CreateTileEntity(Vector2f p_position, TileColor p_color, Coordinates p_coords, TileType p_type, TileState p_state);
    void UpdateTileSprite(Entity p_entity);
    void LoadTextures(RenderWindow& window);
    SDL_Texture* GetTileTexture(TileColor p_color, TileType p_type);

private:
    SDL_Texture* blackGemTexture;
    SDL_Texture* whiteGemTexture;
    SDL_Texture* pinkGemTexture;
    SDL_Texture* blueGemTexture;
    SDL_Texture* orangeGemTexture;
    SDL_Texture* colorlessGemTexture;
};


#endif //__Match3_Entity_Creator_h__