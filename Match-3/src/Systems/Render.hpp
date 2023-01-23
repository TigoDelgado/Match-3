#ifndef __Match3_System_Render_h__
#define __Match3_System_Render_h__

#include "ECS/System.hpp"

#include "RenderWindow.hpp"

class RenderSystem : public System
{
public:
    void Init() override;
	void Update(RenderWindow& window);
private:
    
};


#endif //__Match3_System_Render_h__