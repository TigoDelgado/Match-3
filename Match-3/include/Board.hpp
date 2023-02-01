#ifndef __Match3_Board_h__
#define __Match3_Board_h__

#include <vector>
#include <algorithm>

#include "ECS/Definitions.hpp"
#include "EntityCreator.hpp"
#include "Math2D.hpp"

// Matches can have different shapes and effects
enum MatchType
{
    VerticalMatch,
    HorizontalMatch,
    MixedMatch,
    SpecialMatch
};

// Represents one match (3+ adjacent tiles of the same color) on the board grid
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
    ~Board();

    /* --------------------------- Grid Funcitonality ---------------------------- */

    void PopulateBoard(std::vector<TileColor> p_tileColors);
    void InsertTile(Coordinates p_coords, TileColor p_color, TileType p_type);


    /* ------------------------------ Swapping Tiles ------------------------------ */

    bool CanSwap(Entity p_entityOne, Entity p_entityTwo);
    void SwapTiles(Entity p_tileOne, Entity p_tileTwo);


    /* ------------------------------- Clearing Loop ------------------------------- */

    bool CheckMatches();
    void ClearMatches();
    void SpawnTiles(std::vector<TileColor> p_tileColors);
    void ResetScore();


    /* ------------------------------- Basic Functions ------------------------------- */

    bool BelongsToBoard(Entity p_entity);
    void AddSelected(Entity entity);
    void RemoveSelected(Entity entity);


    /* ----------------------------------- Getters ----------------------------------- */

    int GetScore() {return score;}
    Vector2f GetBackgroundPosition();
    Vector2f GetBackgroundSize();
    Entity GetEntityFromCoords(Coordinates p_coords);
    Vector2f GetPositionFromCoords(Coordinates p_coords);
    Coordinates GetEntityCoords(Entity p_entity);

private:
    std::vector<std::vector<Entity>> grid;

    Vector2f position;

    int cols;    
    int rows;
    Vector2f size;

    Coordinates swappedTiles[2];

    std::vector<Match> GetVerticalMatches();
    std::vector<Match> GetHorizontalMatches();
    bool TilesSameColor(std::vector<Coordinates> p_tiles, TileColor p_color);
    bool MatchesCollide(Match p_match1, Match p_match2, std::vector<Coordinates>& p_jointTiles);
    std::vector<Match> currentMatches;

    void ActivateSpecial(Coordinates p_tile, std::vector<Match>& p_specialMatches);

    int score = 0;
    float combo = 1;
    int CalculateScore(Match match);

    EntityCreator entityCreator;
};

#endif //__Match3_Board_h__