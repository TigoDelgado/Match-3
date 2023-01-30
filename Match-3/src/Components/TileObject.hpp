#ifndef __Match3_Component_Tile_Object_h__
#define __Match3_Component_Tile_Object_h__

#include "Math2D.hpp"

enum TileColor
{
    Black,
    Blue,
    Green,
    Magenta,
    Purple,
    White,
    Yellow,
    Colorless
};

enum TileType
{
    Normal,
    Vertical,
    Horizontal,
    Exploding,
    BigExploding,
    Consuming
};

struct TileObject
{
    TileColor color;
    TileType type;
    Coordinates coords;
};

#endif //__Match3_Component_Tile_Object_h__