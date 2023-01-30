#include "Board.hpp"
#include "GameDefinitions.hpp"

#include "Components/Translate.hpp"
#include "Components/Shrinking.hpp"
#include "Components/EntityDestruction.hpp"
#include "Components/ClearedTile.hpp"
#include "Components/Selected.hpp"
#include "Components/SpriteAnimation.hpp"

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

void Board::InsertTile(Coordinates p_coords, TileColor p_color, TileType p_type)
{
    Entity entity = grid[p_coords.x][p_coords.y];
    TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

    tileObject.color = p_color;
    tileObject.type = p_type;

    entityCreator.UpdateTileSprite(entity);
}

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

                ecsManager.AddComponent(entity, Translate{GetPositionFromCoords(newCoords), TILE_FALL_SPEED, TILE_ACCELERATION, TILE_MAX_SPEED});

                grid[oldCoords.x][oldCoords.y] = NULL_ENTITY;
                grid[newCoords.x][newCoords.y] = entity;
            }
        }

        for (int offset = count; offset > 0; offset--)
        {
            TileColor randColor = p_tileColors[rand() % p_tileColors.size()];
            
            Coordinates coords{x, count - offset};                              // Coordinates after falling
            Coordinates spawnCoords{x, -offset};
            
            Vector2f tilePosition = GetPositionFromCoords(spawnCoords);         // Screen position based on spawn coordinates
             
            Entity entity = entityCreator.CreateTileEntity(tilePosition, randColor, coords, TileType::Normal);
            
            ecsManager.AddComponent(entity, Translate{GetPositionFromCoords(coords), TILE_FALL_SPEED, TILE_ACCELERATION, TILE_MAX_SPEED});
            
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

    ecsManager.AddComponent(grid[c1.x][c1.y], Translate{GetPositionFromCoords(c1), TILE_SWAP_SPEED, 0, TILE_SWAP_SPEED});
    ecsManager.AddComponent(grid[c2.x][c2.y], Translate{GetPositionFromCoords(c2), TILE_SWAP_SPEED, 0, TILE_SWAP_SPEED});

    swappedTiles[0] = c1;
    swappedTiles[1] = c2;
}

bool Board::CheckMatches()
{
    currentMatches.clear();

    std::vector<Match> verticalMatches = GetVerticalMatches();
    std::vector<Match> horizontalMatches = GetHorizontalMatches();

    verticalMatches.insert(verticalMatches.end(), horizontalMatches.begin(), horizontalMatches.end());
    currentMatches = verticalMatches;

    std::size_t i = 0;

    while (i < currentMatches.size())
    {
        Match match1 = currentMatches[i];
        Match collidingMatch{MatchType::MixedMatch, match1.count, match1.tiles};
        bool collided = false;

        std::vector<Coordinates> jointTiles;

        for (std::size_t j = i+1; j < currentMatches.size(); j++)
        {
            Match match2 = currentMatches[j];

            jointTiles.clear();

            if (MatchesCollide(match1, match2, jointTiles))
            {
                collided = true;
                collidingMatch.count = jointTiles.size();
                collidingMatch.tiles = jointTiles;
                currentMatches.erase(currentMatches.begin() + j);
            }
        }

        if (collided)
        {
            currentMatches.erase(currentMatches.begin() + i);
            currentMatches.push_back(collidingMatch);
        }
        else
        {
            i++;
        }
    }

    // std::cout << std::endl << "________________________________________________________" << std::endl;
    // for (Match match : currentMatches)
    // {
    //     match.print();
    //     std::cout << std::endl;
    // }
    // std::cout << "________________________________________________________" << std::endl;

    return (currentMatches.size() > 0);
}

bool Board::MatchesCollide(Match p_match1, Match p_match2, std::vector<Coordinates>& p_jointTiles)
{
    bool collide = false;
    Coordinates collideTile;

    for (Coordinates tile1 : p_match1.tiles)
    {
        p_jointTiles.push_back(tile1);              // tile1 is joint tile if matches collide
        for (Coordinates tile2 : p_match2.tiles)
        {
            if (tile1 == tile2)
            {
                collide = true;                     // Matches collide - we don't push back tile2 because it was already pushed
                collideTile = tile1;
            }
        }
    }
    if (!collide)
    {
        return false;
    } 
    else    // collided
    {
        for (Coordinates tile : p_match2.tiles)
        {
            if (!(tile == collideTile))
            {
                p_jointTiles.push_back(tile);
            }

        }
    }
    return true;
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

        std::sort(currentMatches.begin(), currentMatches.end(), Match::compareMatches); // Leave bigger matches for last (higher score)

        for (Match match: currentMatches)
        {
            if (match.count > 3 && match.type != MatchType::SpecialMatch)               // Match will create a special tile
            {
                int offset = rand() % 2;                                                // Randomize center of even sized matches 
                Coordinates coords = match.tiles[(match.tiles.size() - offset) / 2];    // Get tile at center of match

                for (Coordinates tile : match.tiles)
                {
                    if (tile == swappedTiles[1])                    // If match includes swapped tiles, prioritize them
                    {   
                        coords = swappedTiles[1];  
                        swappedTiles[1] = Coordinates{-1, -1};      // If other matches appear here, ignore
                    }         
                    if (tile == swappedTiles[0])                    // Prioritize first selection
                    {                   
                        coords = swappedTiles[0];
                        swappedTiles[0] = Coordinates{-1, -1};      // If other matches appear here, ignore          
                    } 
                }
                
                Entity entity = grid[coords.x][coords.y];
                
                if (!tilesCleared[coords.x][coords.y] && entity != NULL_ENTITY)     // Tile not cleared yet
                {
                    TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

                    if (tileObject.type != TileType::Normal)            // Transforming special tile
                    {
                        ActivateSpecial(coords, specialMatches);        // Activate its ability before transforming
                    }

                    TileType type;

                    if (match.count == 4 && match.type == MatchType::VerticalMatch)
                    {
                        // Create Vertical Tile
                        type = TileType::Vertical;
                    } 
                    else if (match.count == 4 && match.type == MatchType::HorizontalMatch)
                    {
                        // Create Horizontal Tile
                        type = TileType::Horizontal;
                    } 
                    else if (match.count >= 5 && (match.type == MatchType::VerticalMatch || match.type == MatchType::HorizontalMatch))
                    {
                        // Create Consuming Tile
                        type = TileType::Consuming;
                    }
                    else if (match.count < 8 && match.type == MatchType::MixedMatch)
                    {
                        // Create Exploding Tile
                        type = TileType::Exploding;
                    }
                    else if (match.count >= 8 && match.type == MatchType::MixedMatch)
                    {
                        // Create Big Exploding Tile
                        type = TileType::BigExploding;
                    } 

                    // TODO else throw exception

                    tileObject.type = type;

                    entityCreator.UpdateTileSprite(entity);

                    tilesCleared[coords.x][coords.y] = true; 
                
                }
            }
            

            for (Coordinates tile: match.tiles)                             // Clear remaining match tiles
            {
                Entity entity = grid[tile.x][tile.y];

                if (!tilesCleared[tile.x][tile.y] && entity != NULL_ENTITY) // Tile not cleared yet
                {
                    
                    TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

                    if (tileObject.type != TileType::Normal)                // Special tile activated
                    {
                        ActivateSpecial(tile, specialMatches);              // Create special tiles matches --> specialMatches
                    }

                    ecsManager.AddComponent<Shrinking>(entity, Shrinking{TILE_CLEAR_SCALE, TILE_CLEAR_SPEED, TILE_CLEAR_ACCELERATION});
                    ecsManager.AddComponent<ClearedTile>(entity, ClearedTile{});

                    tilesCleared[tile.x][tile.y] = true;
                    grid[tile.x][tile.y] = NULL_ENTITY;
                }
                // Else: tile already cleared - do nothing.
            }
            int matchScore = CalculateScore(match);
            score += matchScore * combo;

            // if (score == 0) std::cout << std::endl << "=====================================================================" << std::endl;
            // std::cout << std::endl;
            // std::cout << "Match: " << matchScore << std::endl;
            // std::cout << "Combo: " << combo << std::endl;
            // std::cout << "Points: " << matchScore * combo << std::endl;
            // std::cout << "Score: " << score << std::endl;
            // std::cout << "---------------------------------------------------------------------" << std::endl;

            combo += COMBO_AMPLIFIER;
        }
    }

    currentMatches.clear();
}


void Board::ActivateSpecial(Coordinates p_tile, std::vector<Match>& p_specialMatches)
{
    Entity entity = grid[p_tile.x][p_tile.y];
    TileObject& tileObject = ecsManager.GetComponent<TileObject>(entity);

    std::vector<Coordinates> specialCoordinates;
    int count = 0;

    if (tileObject.type == TileType::Vertical)          // Create column-high match
    {
        for (int y = 0; y < rows; y++)
        {
            specialCoordinates.push_back(Coordinates{p_tile.x,y});
            count++;
        }
    }
    else if (tileObject.type == TileType::Horizontal)    // Create row-wide match
    {
        for (int x = 0; x < cols; x++)
        {
            specialCoordinates.push_back(Coordinates{x,p_tile.y});
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
    else if (tileObject.type == TileType::Exploding)                // Create small square match
    {
        for (int xOffset = -1; xOffset <= 1; xOffset++)
        {
            int x = p_tile.x + xOffset;
            if (0 <= x && x < cols)                                 // Make sure tile exists
            {
                for (int yOffset = -1; yOffset <= 1; yOffset++)
                {
                    int y = p_tile.y + yOffset;
                    if (0 <= y && y < rows)                         // Make sure tile exists
                    {
                        specialCoordinates.push_back(Coordinates{x,y});
                        count++;
                    }
                }
            }

        }
    }
    else if (tileObject.type == TileType::BigExploding)             // Create big explosion match
    {
        for (int xOffset = -2; xOffset <= 2; xOffset++)
        {
            int x = p_tile.x + xOffset;
            if (0 <= x && x < cols)                                 // Make sure tile exists
            {
                for (int yOffset = -2; yOffset <= 2; yOffset++)
                {
                    int y = p_tile.y + yOffset;
                    if (0 <= y && y < rows)                         // Make sure tile exists
                    {
                        specialCoordinates.push_back(Coordinates{x,y});
                        count++;
                    }
                }
            }

        }
    }

    Match specialMatch{MatchType::SpecialMatch, count, specialCoordinates};
    p_specialMatches.push_back(specialMatch);
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

int Board::CalculateScore(Match match)
{
    return match.count * 100;
}

void Board::ResetScore()
{
    score = 0;
    combo = 1;
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