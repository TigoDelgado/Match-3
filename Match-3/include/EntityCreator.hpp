#ifndef __Match3_Entity_Creator_h__
#define __Match3_Entity_Creator_h__

#include "RenderWindow.hpp"
#include "Math2D.hpp"

#include "ECS/Manager.hpp"
#include "ECS/Definitions.hpp"

#include "Components/TileObject.hpp"
#include "Components/Position.hpp"
#include "Components/Sprite.hpp"

class EntityCreator
{
public: 
    Entity CreateTileEntity(Vector2f p_position, TileObject p_tileObject);
    void LoadTextures(RenderWindow& window);
    SDL_Texture* GetTexture(TileObject p_tileObject);

private:
    SDL_Texture* blackGemTexture;
    SDL_Texture* whiteGemTexture;
    SDL_Texture* pinkGemTexture;
    SDL_Texture* blueGemTexture;
    SDL_Texture* orangeGemTexture;
    SDL_Texture* colorlessGemTexture;
};


#endif //__Match3_Entity_Creator_h__