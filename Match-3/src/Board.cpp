#include "Board.hpp"
#include "GameDefinitions.hpp"

Board::Board(Vector2f p_position, int p_rows, int p_cols, EntityCreator& p_entityCreator)
    :position(p_position), rows(p_rows), cols(p_cols), entityCreator(p_entityCreator)
{
    size = rows * cols;
    grid.reserve(size);
}

void Board::PopulateBoard(std::vector<TileObject> p_tileObjects)
{
    int size = p_tileObjects.size();
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            TileObject randomTileObject = p_tileObjects[rand() % size];
            float pos_x = position.x + x * (TILE_WIDTH + TILE_MARGIN);
            float pos_y = position.y + y * (TILE_HEIGHT + TILE_MARGIN);
            entityCreator.CreateTileEntity(Vector2f{pos_x, pos_y}, randomTileObject);
        }
    }
};


void Board::SwapTiles(Coordinates p_firstTile, Coordinates p_secondTile)
{
    int firstIndex = GetIndexFromCoords(p_firstTile);
    int secondIndex = GetIndexFromCoords(p_firstTile);

    Entity tempEntity = grid[firstIndex];
    grid[firstIndex] = grid[secondIndex];
    grid[secondIndex] = tempEntity;
}

int Board::GetIndexFromPosition(Vector2f p_position) 
{
    return -1;
}

int Board::GetIndexFromCoords(Coordinates p_coordinates) {return p_coordinates.x + p_coordinates.y * cols;}