#include "Board.hpp"
#include "GameDefinitions.hpp"

#include "Components/Translate.hpp"
#include "Components/Destruction.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

Board::Board(Vector2f p_position, int p_cols, int p_rows, EntityCreator& p_entityCreator)
    :position(p_position), cols(p_cols), rows(p_rows), entityCreator(p_entityCreator)
{
    grid = std::vector<std::vector<Entity>> (cols, std::vector<Entity>(rows, NULL_ENTITY));
}

void Board::PopulateBoard(std::vector<TileColor> p_tileColors)
{
    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < rows; y++)
        {
            TileColor randColor = p_tileColors[rand() % p_tileColors.size()];
            Coordinates coords{x,y};
            Vector2f tilePosition = GetPositionFromCoords(coords);
            Entity entity = entityCreator.CreateTileEntity(tilePosition, randColor, coords, TileType::Normal, TileState::Idle);

            grid[x][y] = entity;
        }
    }

    // FIXME NOT IMPLEMENTED YET --> DO NOT LEAVE LIKE THIS
    while (CheckMatches())
    {
        for (int x = 0; x < cols; x++)
        {
            for (int y = 0; y < rows; y++)
            {
                TileColor randColor = p_tileColors[rand() % p_tileColors.size()];
                Coordinates coords{x,y};
                Vector2f tilePosition = GetPositionFromCoords(coords);
                Entity entity = entityCreator.CreateTileEntity(tilePosition, randColor, coords, TileType::Normal, TileState::Idle);

                ecsManager.DestroyEntity(grid[x][y]);
                grid[x][y] = entity;
            }
        }
    }
};

void Board::SpawnTiles(std::vector<TileColor> p_tileColors)
{
    for (int x = 0; x < cols; x++)              // For each column
    {
        int count = 0;                          // Amount of tile spaces to drop

        for (int y = rows - 1; y >= 0; y--)     // Iterate through column from bottom to top
        {
            Entity entity = grid[x][y];

            if (entity == NULL_ENTITY)          // Empty tile space
            {
                count++;
            }
            else if (count > 0)                 // Tile must drop (count) spaces
            {
                Coordinates oldCoords = GetEntityCoords(entity);
                Coordinates newCoords{oldCoords.x, oldCoords.y + count};

                TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);
                tileObject.coords = newCoords;

                ecsManager.AddComponent(entity, Translate{GetPositionFromCoords(newCoords), TILE_FALL_SPEED});

                grid[oldCoords.x][oldCoords.y] = NULL_ENTITY;
                grid[newCoords.x][newCoords.y] = entity;
            }
        }

        for (int offset = count; offset > 0; offset--)
        {
            TileColor randColor = p_tileColors[rand() % p_tileColors.size()];
            
            Coordinates coords{x, count - offset};                                      // Coordinates after falling
            Coordinates spawnCoords{x, -offset};
            
            Vector2f tilePosition = GetPositionFromCoords(spawnCoords);     // Screen position based on spawn coordinates
             
            Entity entity = entityCreator.CreateTileEntity(tilePosition, randColor, coords, TileType::Normal, TileState::Idle);
            
            ecsManager.AddComponent(entity, Translate{GetPositionFromCoords(coords), TILE_FALL_SPEED});
            
            grid[coords.x][coords.y] = entity;
        }
    }
}


void Board::SwapTiles(Entity p_tileOne, Entity p_tileTwo)
{
    Coordinates c1 = GetEntityCoords(p_tileOne);
    Coordinates c2 = GetEntityCoords(p_tileTwo);

    Entity tempEntity = grid[c1.x][c1.y];
    grid[c1.x][c1.y] = grid[c2.x][c2.y];
    grid[c2.x][c2.y] = tempEntity;

    TileObject& tileObject1 = ecsManager.GetComponent<TileObject>(grid[c1.x][c1.y]);
    tileObject1.coords = c1;

    TileObject& tileObject2 = ecsManager.GetComponent<TileObject>(grid[c2.x][c2.y]);
    tileObject2.coords = c2;

    ecsManager.AddComponent(grid[c1.x][c1.y], Translate{GetPositionFromCoords(c1), TILE_SWAP_SPEED});
    ecsManager.AddComponent(grid[c2.x][c2.y], Translate{GetPositionFromCoords(c2), TILE_SWAP_SPEED});
}

bool Board::CheckMatches()
{
    currentMatches.clear();

    std::vector<Match> verticalMatches = GetVerticalMatches();
    std::vector<Match> horizontalMatches = GetHorizontalMatches();

    SetMatches(verticalMatches, horizontalMatches);

    verticalMatches.insert( verticalMatches.end(), horizontalMatches.begin(), horizontalMatches.end());
    currentMatches = verticalMatches;

    // for (Match m : currentMatches)
    // {
    //     m.print();
    // }

    return (currentMatches.size() > 0);
}



Coordinates Board::GetEntityCoords(Entity p_entity)
{
    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < rows; y++)
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

bool Board::CanSwap(Entity p_entityOne, Entity p_entityTwo)
{
    Coordinates tileOne = GetEntityCoords(p_entityOne);
    Coordinates tileTwo = GetEntityCoords(p_entityTwo);

    if (p_entityOne != NULL_ENTITY && p_entityTwo != NULL_ENTITY)
    {
        if ((tileOne.x == tileTwo.x     && tileOne.y == tileTwo.y - 1   ) ||
            (tileOne.x == tileTwo.x     && tileOne.y == tileTwo.y + 1   ) ||
            (tileOne.x == tileTwo.x - 1 && tileOne.y == tileTwo.y       ) ||
            (tileOne.x == tileTwo.x + 1 && tileOne.y == tileTwo.y       ))
        {
            TileObject& tileOne = ecsManager.GetComponent<TileObject>(p_entityOne);
            TileObject& tileTwo = ecsManager.GetComponent<TileObject>(p_entityTwo);

            return (tileOne.state == TileState::Idle && tileTwo.state == TileState::Idle);
        }
    }
    return false;
}

void Board::ClearMatches()
{
    for (Match match: currentMatches)
    {
        for (Coordinates tile: match.tiles)
        {
            Entity entity = grid[tile.x][tile.y];

            if (entity != NULL_ENTITY)
            {
                
                TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

                if (tileObject.state == TileState::Clearing) 
                {
                    // Tile already being cleared. Do nothing.
                } 
                else
                {
                    ecsManager.AddComponent<Destruction>(entity, Destruction{TILE_CLEAR_SCALE, TILE_CLEAR_SPEED});
                    grid[tile.x][tile.y] = NULL_ENTITY;
                    tileObject.state = TileState::Clearing;
                }
            }
        }
    }
    currentMatches.clear();
}


std::vector<Match> Board::GetVerticalMatches()
{
    std::vector<Match> matches;

    for (int x = 0; x < cols; x++)
    {
        int count = 0;
        TileColor lastColor = TileColor::Colorless;

        for (int y = 0; y < rows; y++)
        {
            Entity entity = grid[x][y];
            TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);
            TileColor color = tileObject.color;
            
            if (entity == NULL_ENTITY || color == Colorless || color != lastColor || y == rows - 1)
            {
                if (y == rows - 1 && color == lastColor) // Add last tile to the match
                {
                    count++;
                    y++;
                }
                
                if (count >= 3)
                {
                    std::vector<Coordinates> tiles;

                    for (int i = count; i > 0; i--)
                    {
                        tiles.push_back(Coordinates{x,y-i});
                    }
                    Match match {MatchType::VerticalMatch, count, tiles};
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

    for (int y = 0; y < rows; y++)
    {
        int count = 0;
        TileColor lastColor = TileColor::Colorless;

        for (int x = 0; x < cols; x++)
        {
            Entity entity = grid[x][y];
            TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);
            TileColor color = tileObject.color;
            
            if (entity == NULL_ENTITY || color == Colorless || color != lastColor || x == cols - 1)
            {
                if (x == cols - 1 && color == lastColor) // Add last tile to the match
                {
                    count++;
                    x++;
                }
                
                if (count >= 3)
                {
                    std::vector<Coordinates> tiles;

                    for (int i = count; i > 0; i--)
                    {
                        tiles.push_back(Coordinates{x-i,y});
                    }
                    Match match {MatchType::HorizontalMatch, count, tiles};
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