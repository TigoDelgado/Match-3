#ifndef __Match3_System_Clear_Tile_h__
#define __Match3_System_Clear_Tile_h__

#include "ECS/System.hpp"
#include "Math2D.hpp"

class ClearTileSystem : public System
{
public:
    void Init() override;
	bool Update(float dt);
private:
    bool Cleared(Vector2f init, Vector2f dest, Vector2f final);
};


#endif //__Match3_System_Clear_Tile_h__