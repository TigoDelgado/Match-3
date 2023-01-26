#include "Board.hpp"
#include "GameDefinitions.hpp"

#include "Components/Translate.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

Board::Board(Vector2f p_position, int p_rows, int p_cols, EntityCreator& p_entityCreator)
    :position(p_position), rows(p_rows), cols(p_cols), entityCreator(p_entityCreator)
{
    grid = std::vector<std::vector<Entity>> (rows, std::vector<Entity>(cols));
}

void Board::PopulateBoard(std::vector<TileColor> p_tileColors)
{
    int size = p_tileColors.size();
    
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            TileColor randColor = p_tileColors[rand() % size];
            Coordinates coords{x,y};
            Vector2f tilePosition = GetPositionFromCoords(coords);
            Entity entity = entityCreator.CreateTileEntity(tilePosition, randColor, coords, TileType::Normal);

            grid[x][y] = entity;
        }
    }
};


void Board::SwapTiles(Coordinates p_1, Coordinates p_2)
{
    std::cout << "Swapping Tiles" << std::endl; 

    Entity tempEntity = grid[p_1.x][p_1.y];
    grid[p_1.x][p_1.y] = grid[p_2.x][p_2.y];
    grid[p_2.x][p_2.y] = tempEntity;

    TileObject& tileObject1 = ecsManager.GetComponent<TileObject>(grid[p_1.x][p_1.y]);
    tileObject1.coords = p_1;

    TileObject& tileObject2 = ecsManager.GetComponent<TileObject>(grid[p_2.x][p_2.y]);
    tileObject2.coords = p_2;

    ecsManager.AddComponent(grid[p_1.x][p_1.y], Translate{GetPositionFromCoords(p_1), 500.0f});
    ecsManager.AddComponent(grid[p_2.x][p_2.y], Translate{GetPositionFromCoords(p_2), 500.0f});

}

Coordinates Board::GetEntityCoords(Entity p_entity)
{
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++)
        {
            Coordinates coords = {x, y};
            if (p_entity == grid[x][y])
            {
                return coords;
            }
        }
    }
    return Coordinates{-1, -1};
}

Entity Board::GetEntityFromCoords(Coordinates p_coords) {return grid[p_coords.x][p_coords.y];}

Vector2f Board::GetPositionFromCoords(Coordinates p_coordinates) 
{
    float pos_x = position.x + p_coordinates.x * (TILE_WIDTH + TILE_MARGIN);
    float pos_y = position.y + p_coordinates.y * (TILE_HEIGHT + TILE_MARGIN);
    return Vector2f{pos_x, pos_y};
}

bool Board::CanSwap(Coordinates p_tileOne, Coordinates p_tileTwo)
{
    if ((p_tileOne.x == p_tileTwo.x && p_tileOne.y == p_tileTwo.y - 1) ||
        (p_tileOne.x == p_tileTwo.x && p_tileOne.y == p_tileTwo.y + 1) ||
        (p_tileOne.x == p_tileTwo.x - 1 && p_tileOne.y == p_tileTwo.y) ||
        (p_tileOne.x == p_tileTwo.x + 1 && p_tileOne.y == p_tileTwo.y) )
        {
            return true;
        }
    else return false;
}