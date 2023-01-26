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
    std::vector<Entity> entities;
    Coordinates start;
    Coordinates end;

    void print() 
    {
        std::cout << "Match type: " << type << " | count: " << count << " | start : [" << start.x << ", " << start.y << "] | end: [" << end.x << ", " << end.y << "]" << std::endl;
    }
};

class Board
{
public:
    Board(Vector2f p_position, int p_rows, int p_cols, EntityCreator& p_entityCreator);

    void PopulateBoard(std::vector<TileColor> p_tileColors);      // populates board with random Tile Entities of desired Tile Types 
    void InsertTile(Entity p_tile, Coordinates p_coords);

    void SwapTiles(Coordinates p_1, Coordinates p_2);

    bool CheckMatches();

    Entity GetEntityFromCoords(Coordinates p_coords);

    Vector2f GetPositionFromCoords(Coordinates p_coords);

    Coordinates GetEntityCoords(Entity p_entity);

    bool CanSwap(Coordinates p_tileOne, Coordinates p_tileTwo);

private:
    Vector2f position;
    int rows;
    int cols;    
    EntityCreator entityCreator;
    std::vector<std::vector<Entity>> grid;
    int size; // TODO remove if not necessary

    std::vector<Match> GetVerticalMatches();
    std::vector<Match> GetHorizontalMatches();
    void SetMatches(std::vector<Match>& p_verticalMatches, std::vector<Match>& p_horizontalMatches);
    std::vector<Match> currentMatches;
};

#endif //__Match3_Board_h__