#ifndef __Match3_Tile_Creator_h__
#define __Match3_Tile_Creator_h__

#include "RenderWindow.hpp"
#include "ECS/Definitions.hpp"
#include "Math2D.hpp"

// Unique ID for each tile type present in the game
// Used to populate the board randomly with specific tiles --> we can even create a board with only 2 colors of gem
enum TileType
{
    BlackGem,
    WhiteGem,
    PinkGem,
    BlueGem,
    OrangeGem,
    Block
};

class TileCreator
{
public: 
    Entity CreateTileEntity(Vector2f position, TileType type);
    void LoadTextures(RenderWindow& window);

private:
    SDL_Texture* blackGemTexture;
    SDL_Texture* whiteGemTexture;
    SDL_Texture* pinkGemTexture;
    SDL_Texture* blueGemTexture;
    SDL_Texture* orangeGemTexture;
};


#endif //__Match3_Tile_Creator_h__