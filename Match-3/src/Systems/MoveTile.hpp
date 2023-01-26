#ifndef __Match3_System_Move_Tile_h__
#define __Match3_System_Move_Tile_h__

#include "ECS/System.hpp"
#include "Math2D.hpp"

class MoveTileSystem : public System
{
public:
    void Init() override;
	bool Update(float dt);
private:
    bool Arrived(Vector2f init, Vector2f dest, Vector2f final);
};


#endif //__Match3_System_Move_Tile_h__