#ifndef __Match3_System_Click_Tile_h__
#define __Match3_System_Click_Tile_h__

#include "ECS/System.hpp"
#include "Math2D.hpp"

class ClickTileSystem : public System
{
public:
    void Init() override;
	bool ClickedEntity(Vector2f p_point, Entity& p_entity);
};


#endif //__Match3_System_Click_Tile_h__