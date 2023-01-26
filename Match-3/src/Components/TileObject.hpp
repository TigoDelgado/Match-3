#ifndef __Match3_Component_Tile_Object_h__
#define __Match3_Component_Tile_Object_h__

#include "Math2D.hpp"

enum TileColor
{
    Black,
    White,
    Pink,
    Blue,
    Orange,
    Colorless
};

enum TileType
{
    Normal,
    Vertical,
    Horizontal,
    Explosion,
    Consuming
};

enum TileState
{
    Idle,
    Swapping,
    Gravitating,
    Clearing
};

struct TileObject
{
    TileColor color;
    Coordinates coords;
    TileState state;
};

#endif //__Match3_Component_Tile_Object_h__