#ifndef __Match3_Component_Tile_Object_h__
#define __Match3_Component_Tile_Object_h__

enum TileColor
{
    Black,
    White,
    Pink,
    Blue,
    Orange,
    Colorless
};

struct TileObject
{
    TileColor color;
    bool isMovable;
};

#endif //__Match3_Component_Tile_Object_h__