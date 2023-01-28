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
    Exploding,
    Consuming
};

struct TileObject
{
    TileColor color;
    TileType type;
    Coordinates coords;
};

#endif //__Match3_Component_Tile_Object_h__