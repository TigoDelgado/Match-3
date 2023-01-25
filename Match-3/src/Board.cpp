#include "Board.hpp"

Board::Board(Vector2f p_position, int p_rows, int p_cols, std::vector<TileType> p_tileTypes)
    :position(p_position), rows(p_rows), cols(p_cols), tileTypes(p_tileTypes)
{
    size = rows * cols;
    grid.reserve(size);
}

void Board::PopulateBoard(TileCreator& tileCreator)
{
    int size = tileTypes.size();
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            TileType randomType = tileTypes[rand() % size];
            float pos_x = position.x + x * 50;
            float pos_y = position.y + y * 50;;
            tileCreator.CreateTileEntity(Vector2f{pos_x, pos_y}, randomType);
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

int Board::GetIndexFromCoords(Coordinates p_coordinates) {return p_coordinates.x + p_coordinates.y * cols;}