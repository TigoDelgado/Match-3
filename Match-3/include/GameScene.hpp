#ifndef __Match3_Game_Scene_h__
#define __Match3_Game_Scene_h__

#include <SDL2/SDL.h>

// Game Scene interface
class GameScene
{
public:
    //Main loop functions
    virtual void HandleEvent(SDL_Event& event) = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual bool ChangeScene() = 0;
    virtual GameScene* GetNextScene() = 0;

    virtual ~GameScene(){};
};

#endif //__Match3_Game_Scene_h__