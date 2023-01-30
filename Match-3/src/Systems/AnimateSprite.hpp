#ifndef __Match3_System_Animate_Sprite_h__
#define __Match3_System_Animate_Sprite_h__

#include "ECS/System.hpp"
#include "Math2D.hpp"

class AnimateSpriteSystem : public System
{
public:
    void Init() override;
	void Update(float dt);

private:
};


#endif //__Match3_System_Animate_Sprite_h__