#ifndef __Match3_Board_h__
#define __Match3_Board_h__

#include <vector>

#include "ECS/Definitions.hpp"
#include "EntityCreator.hpp"
#include "Math2D.hpp"

class Board
{
public:
    Board(Vector2f p_position, int p_rows, int p_cols, EntityCreator& p_entityCreator);

    void PopulateBoard(std::vector<TileColor> p_tileColors);      // generates populates board with random Tile Entities of desired Tile Types 
    void InsertTile(Entity p_tile, Coordinates p_coords);

    void SwapTiles(Coordinates p_1, Coordinates p_2);

    Entity GetEntityFromCoords(Coordinates p_coords);

    Vector2f GetPositionFromCoords(Coordinates p_coords);

    Coordinates GetEntityCoords(Entity p_entity);

    static bool CanSwap(Coordinates p_tileOne, Coordinates p_tileTwo);

private:
    Vector2f position;
    int rows;
    int cols;    
    EntityCreator entityCreator;
    std::vector<std::vector<Entity>> grid;
    int size; // TODO remove if not necessary
};

#endif //__Match3_Board_h__