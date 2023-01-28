#include "Board.hpp"
#include "GameDefinitions.hpp"

#include "Components/Translate.hpp"
#include "Components/Destruction.hpp"
#include "Components/Selected.hpp"

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
            Entity entity = entityCreator.CreateTileEntity(tilePosition, randColor, coords, TileType::Normal);

            grid[x][y] = entity;
        }
    }

    while (CheckMatches())
    {
        for (Match match : currentMatches)
        {
            Coordinates centerTile = match.tiles[match.count / 2];

            Entity entity = grid[centerTile.x][centerTile.y];

            TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

            TileColor newColor = p_tileColors[rand() % p_tileColors.size()];
            
            while (tileObject.color == newColor)
            {
                newColor = p_tileColors[rand() % p_tileColors.size()];
            }

            tileObject.color = newColor;

            entityCreator.UpdateTileSprite(entity);

            // std::cout << "Changed color of entity " << entity << " on tile [" << centerTile.x << ", " << centerTile.y << "]" << std::endl;
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
             
            Entity entity = entityCreator.CreateTileEntity(tilePosition, randColor, coords, TileType::Normal);
            
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

    if ((tileOne.x == tileTwo.x     && tileOne.y == tileTwo.y - 1   ) ||
        (tileOne.x == tileTwo.x     && tileOne.y == tileTwo.y + 1   ) ||
        (tileOne.x == tileTwo.x - 1 && tileOne.y == tileTwo.y       ) ||
        (tileOne.x == tileTwo.x + 1 && tileOne.y == tileTwo.y       ))
    {
        return BelongsToBoard(p_entityOne) && BelongsToBoard(p_entityTwo);
    }
    return false;
}

bool Board::BelongsToBoard(Entity p_entity)         // FIXME should this be done?
{
    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < rows; y++)
        {
            Entity testEntity = grid[x][y];
            if (p_entity == testEntity)
            {
                return true;
            }
        }
    }
    return false;
}

void Board::ClearMatches()
{
    std::vector<Match> specialMatches;
    bool firstLoop = true;

    std::vector<std::vector<bool>> tilesCleared = std::vector<std::vector<bool>>(cols, std::vector<bool>(rows, false));

    while (firstLoop || specialMatches.size() > 0)
    {
        firstLoop = false;
        currentMatches.insert( currentMatches.end(), specialMatches.begin(), specialMatches.end());
        specialMatches.clear();

        for (Match match: currentMatches)
        {
            Coordinates coords = match.tiles[match.tiles.size() / 2];
            Entity entity = grid[coords.x][coords.y];
            TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

            if (match.count >= 4 && TilesSameColor(match.tiles, tileObject.color) 
                && (match.type == MatchType::VerticalMatch || match.type == MatchType::HorizontalMatch))   // Create special tile if all of same color
            {
                TileType type;
                if (match.count == 4 && match.type == MatchType::VerticalMatch)
                {
                    type = TileType::Vertical;
                } 
                if (match.count == 4 && match.type == MatchType::HorizontalMatch)
                {
                    type = TileType::Horizontal;
                } 
                if (match.count >= 5)
                {
                    type = TileType::Consuming;
                } 

                tileObject.type = type;

                entityCreator.UpdateTileSprite(entity);

                // std::cout << "Created a special tile at [" << coords.x << ", " << coords.y << "]" << std::endl;

                tilesCleared[coords.x][coords.y] = true; 
            }


            for (Coordinates tile: match.tiles)
            {
                Entity entity = grid[tile.x][tile.y];

                if (!tilesCleared[tile.x][tile.y] && entity != NULL_ENTITY) // Tile not cleared yet
                {
                    
                    TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

                    if (tileObject.type != TileType::Normal)                // Create special tiles matches --> activate power
                    {
                        std::vector<Coordinates> specialCoordinates;
                        int count = 0;

                        if (tileObject.type == TileType::Vertical)          // Create column-high match
                        {
                            for (int y = 0; y < rows; y++)
                            {
                                specialCoordinates.push_back(Coordinates{tile.x,y});
                                count++;
                            }
                        }
                        else if (tileObject.type == TileType::Horizontal)    // Create row-wide match
                        {
                            for (int x = 0; x < cols; x++)
                            {
                                specialCoordinates.push_back(Coordinates{x,tile.y});
                                count++;
                            }
                        }
                        else if (tileObject.type == TileType::Consuming)     // Create match with every tile of same color
                        {
                            for (int x = 0; x < cols; x++)
                            {
                                for (int y = 0; y < rows; y++)
                                {
                                    Entity testedEntity = grid[x][y];

                                    TileObject& testedTileObject = ecsManager.GetComponent<TileObject>(testedEntity);
                                        
                                    if (tileObject.color == testedTileObject.color)
                                    {
                                        specialCoordinates.push_back(Coordinates{x,y});
                                            count++;
                                    }
                                }
                            }
                        }
                        Match specialMatch{MatchType::MixedMatch, count, specialCoordinates};

                        specialMatches.push_back(specialMatch);
                    }

                    ecsManager.AddComponent<Destruction>(entity, Destruction{TILE_CLEAR_SCALE, TILE_CLEAR_SPEED});
                    tilesCleared[tile.x][tile.y] = true;
                    grid[tile.x][tile.y] = NULL_ENTITY;
                }
                // Else: tile already cleared - do nothing.
            }
        }
    }

    currentMatches.clear();
}

bool Board::TilesSameColor(std::vector<Coordinates> p_tiles, TileColor p_color)
{
    if (p_color == TileColor::Colorless) return false;

    for (Coordinates coords : p_tiles)
    {
        Entity entity = grid[coords.x][coords.y];
        if (entity != NULL_ENTITY)
        {
            Entity entity = grid[coords.x][coords.y];
            TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

            if (tileObject.color != p_color) return false;
        }
    }

    return true;       
}

void Board::AddSelected(Entity entity)
{
    ecsManager.AddComponent<Selected>(entity, Selected{});
    Transform& transform = ecsManager.GetComponent<Transform>(entity);
    transform.rotation = 0.0f;
    transform.scale = 0.95f;
}


void Board::RemoveSelected(Entity entity)
{
    ecsManager.RemoveComponent<Selected>(entity);
    Transform& transform = ecsManager.GetComponent<Transform>(entity);
    transform.rotation = 0.0f;
    transform.scale = 1.0f;
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