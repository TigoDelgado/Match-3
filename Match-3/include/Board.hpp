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

    void PopulateBoard(std::vector<TileObject> p_tileObjects);      // generates populates board with random Tile Entities of desired Tile Types 
    void InsertTile(Entity p_tile, Coordinates p_coords);

    void SwapTiles(Coordinates p_firstTile, Coordinates p_secondTile);

    void ClearMatches();
    void SpawnTiles();
    void GravitateTiles();

    int GetIndexFromPosition(Vector2f p_position);
    int GetIndexFromCoords(Coordinates p_coords);

    std::vector<Entity> getGrid() {return grid;}

private:
    Vector2f position;
    int rows;
    int cols;    
    EntityCreator entityCreator;
    std::vector<Entity> grid;
    int size; // TODO remove if not necessary
};

#endif //__Match3_Board_h__