#include "Board.hpp"
#include "GameDefinitions.hpp"

#include "Components/Translate.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

Board::Board(Vector2f p_position, int p_rows, int p_cols, EntityCreator& p_entityCreator)
    :position(p_position), rows(p_rows), cols(p_cols), entityCreator(p_entityCreator)
{
    grid = std::vector<std::vector<Entity>> (rows, std::vector<Entity>(cols, NULL_ENTITY));

    std::cout << "grid[0][0] --> " << grid[0][0] << std::endl;
    std::cout << "grid[2][2] --> " << grid[2][2] << std::endl;
    std::cout << "grid[3][4] --> " << grid[3][4] << std::endl;
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

    // FIXME NOT IMPLEMENTED YET --> DO NOT LEAVE LIKE THIS
    while (CheckMatches())
    {
        for (int x = 0; x < rows; x++)
        {
        for (int y = 0; y < cols; y++)
        {
            TileColor randColor = p_tileColors[rand() % size];
            Coordinates coords{x,y};
            Vector2f tilePosition = GetPositionFromCoords(coords);
            Entity entity = entityCreator.CreateTileEntity(tilePosition, randColor, coords, TileType::Normal);

            ecsManager.DestroyEntity(grid[x][y]);
            
            grid[x][y] = entity;
        }
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

    ecsManager.AddComponent(grid[p_1.x][p_1.y], Translate{GetPositionFromCoords(p_1), 400.0f});
    ecsManager.AddComponent(grid[p_2.x][p_2.y], Translate{GetPositionFromCoords(p_2), 400.0f});
}

bool Board::CheckMatches()
{
    currentMatches.clear();

    std::vector<Match> verticalMatches = GetVerticalMatches();
    std::vector<Match> horizontalMatches = GetHorizontalMatches();

    SetMatches(verticalMatches, horizontalMatches);

    verticalMatches.insert( verticalMatches.end(), horizontalMatches.begin(), horizontalMatches.end());
    currentMatches = verticalMatches;

    for (Match m : currentMatches)
    {
        m.print();
    }

    return (currentMatches.size() > 0);
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


std::vector<Match> Board::GetVerticalMatches()
{
    std::vector<Match> matches;

    for (int x = 0; x < rows; x++)
    {
        int count = 0;
        TileColor lastColor = TileColor::Colorless;

        for (int y = 0; y < cols; y++)
        {
            Entity entity = grid[x][y];
            TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);
            TileColor color = tileObject.color;
            
            if (color == Colorless || color != lastColor || y == cols - 1)
            {
                if (y == cols - 1 && color == lastColor) // Add last tile to the match
                {
                    count++;
                    y++;
                }
                
                if (count >= 3)
                {
                    std::vector<Entity> entities;

                    for (int i = count; i > 0; i--)
                    {
                        entities.push_back(grid[x][y-i]);
                    }
                    Match match {MatchType::VerticalMatch, count, entities, Coordinates{x, y-count}, Coordinates{x, y-1}};
                    matches.push_back(match);
                }
                count = 1;
                lastColor = color;
            }
            else
            {
                count++;
            }
        }
    }
    return matches;
}


std::vector<Match> Board::GetHorizontalMatches()
{
    std::vector<Match> matches;

    for (int y = 0; y < cols; y++)
    {
        int count = 0;
        TileColor lastColor = TileColor::Colorless;

        for (int x = 0; x < rows; x++)
        {
            Entity entity = grid[x][y];
            TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);
            TileColor color = tileObject.color;
            
            if (color == Colorless || color != lastColor || x == rows - 1)
            {
                if (x == cols - 1 && color == lastColor) // Add last tile to the match
                {
                    count++;
                    x++;
                }
                
                if (count >= 3)
                {
                    std::vector<Entity> entities;

                    for (int i = count; i > 0; i--)
                    {
                        entities.push_back(grid[x-i][y]);
                    }
                    Match match {MatchType::HorizontalMatch, count, entities, Coordinates{x-count, y}, Coordinates{x-1,y}};
                    matches.push_back(match);
                }
                count = 1;
                lastColor = color;
            }
            else
            {
                count++;
            }
        }
    }
    return matches;
}

void Board::SetMatches(std::vector<Match>& p_verticalMatches, std::vector<Match>& p_horizontalMatches)
{
    std::vector<Match> matches;

    // TODO concatenate matches!
}