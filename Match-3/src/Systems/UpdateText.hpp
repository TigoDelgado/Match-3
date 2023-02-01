#ifndef __Match3_System_Update_Text_h__
#define __Match3_System_Update_Text_h__

#include "ECS/System.hpp"

#include "RenderWindow.hpp"

class UpdateTextSystem : public System
{
public:
    void Init() override;
	void Update(RenderWindow& window);
private:
    
};


#endif //__Match3_System_Update_Text_h__