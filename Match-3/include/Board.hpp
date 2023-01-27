#ifndef __Match3_Board_h__
#define __Match3_Board_h__

#include <vector>

#include "ECS/Definitions.hpp"
#include "EntityCreator.hpp"
#include "Math2D.hpp"

enum MatchType
{
    VerticalMatch,
    HorizontalMatch,
    MixedMatch
};

struct Match
{
    MatchType type;
    int count;
    std::vector<Coordinates> tiles;

    void print() 
    {
        std::cout << "Match type: " << type << " | count: " << count << " | start : [" << tiles[0].x << ", " << tiles[0].y << "] | end: [" << tiles[tiles.size()-1].x << ", " << tiles[tiles.size()-1].y << "]" << std::endl;
    }
};

class Board
{
public:
    Board(Vector2f p_position, int p_cols, int p_rows, EntityCreator& p_entityCreator);

    void PopulateBoard(std::vector<TileColor> p_tileColors);      // populates board with random Tile Entities of desired Tile Types 

    void SpawnTiles(std::vector<TileColor> p_tileColors);

    void InsertTile(Entity p_tile, Coordinates p_coords);

    void SwapTiles(Entity p_tileOne, Entity p_tileTwo);

    bool CheckMatches();

    Entity GetEntityFromCoords(Coordinates p_coords);

    Vector2f GetPositionFromCoords(Coordinates p_coords);

    Coordinates GetEntityCoords(Entity p_entity);

    bool CanSwap(Entity p_entityOne, Entity p_entityTwo);

    void ClearMatches();

    void AddSelected(Entity entity);
    void RemoveSelected(Entity entity);

private:
    Vector2f position;
    int cols;    
    int rows;
    EntityCreator entityCreator;
    std::vector<std::vector<Entity>> grid;

    std::vector<Match> GetVerticalMatches();
    std::vector<Match> GetHorizontalMatches();
    void SetMatches(std::vector<Match>& p_verticalMatches, std::vector<Match>& p_horizontalMatches);
    std::vector<Match> currentMatches;
};

#endif //__Match3_Board_h__