#ifndef __Match3_Board_h__
#define __Match3_Board_h__

#include <vector>
#include <algorithm>

#include "ECS/Definitions.hpp"
#include "EntityCreator.hpp"
#include "Math2D.hpp"

enum MatchType
{
    VerticalMatch,
    HorizontalMatch,
    MixedMatch,
    SpecialMatch
};

struct Match
{
    MatchType type;
    int count;
    std::vector<Coordinates> tiles;

    static bool compareMatches(Match p_match1, Match p_match2)
    {
        return (p_match1.count < p_match2.count);
    }

    void print() 
    {
        std::cout << "Match type: " << type << " | count: " << count << " | start : [" << tiles[0].x << ", " << tiles[0].y << "] | end: [" << tiles[tiles.size()-1].x << ", " << tiles[tiles.size()-1].y << "]" << std::endl;
        std::cout << "Tiles: ";
        for (Coordinates tile: tiles)
        {
            std::cout << "[" << tile.x << ", " << tile.y << "]  ";
        }
        std::cout << std::endl;
    }
};

class Board
{
public:
    Board(Vector2f p_position, int p_cols, int p_rows, EntityCreator& p_entityCreator);

    void PopulateBoard(std::vector<TileColor> p_tileColors);      // populates board with random Tile Entities of desired Tile Types 

    void SpawnTiles(std::vector<TileColor> p_tileColors);

    void InsertTile(Coordinates p_coords, TileColor p_color, TileType p_type);

    void SwapTiles(Entity p_tileOne, Entity p_tileTwo);

    bool CheckMatches();

    Entity GetEntityFromCoords(Coordinates p_coords);

    Vector2f GetPositionFromCoords(Coordinates p_coords);

    Coordinates GetEntityCoords(Entity p_entity);

    bool CanSwap(Entity p_entityOne, Entity p_entityTwo);
    bool BelongsToBoard(Entity p_entity);
    bool MatchesCollide(Match p_match1, Match p_match2, std::vector<Coordinates>& p_jointTiles);

    void ClearMatches();
    void ActivateSpecial(Coordinates p_tile, std::vector<Match>& p_specialMatches);
    bool TilesSameColor(std::vector<Coordinates> p_tiles, TileColor p_color);
    int CalculateScore(Match match);
    void ResetScore();

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
    std::vector<Match> currentMatches;

    Coordinates swappedTiles[2];

    int score = 0;
    float combo = 1;
};

#endif //__Match3_Board_h__