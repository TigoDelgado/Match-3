# Match-3

## The challenge

I was challenged to develop a Match 3 game in C++ using the libSDL library. This is my first time actually working with SDL so it has been a great learning experience.
I also decided to take this oportunity to explore more data driven approaches, namely the ECS architecture - Entity Component System. 

## Core gameplay loop

Our game loop is comprised of two main loops: 
- waiting for the player to select two valid tiles to swap (yellow)
- clearing tile matches and spawning new tiles (blue)

![](https://github.com/TigoDelgado/Match-3/blob/master/Core%20gameplay%20loop.jpg)

This diagram translates pretty closely to how our code works. The main function calls Game.Run(), which handles the setup and running of the main game loop --
calculating delta time between frames for example. Game creates a game Scene, which will handle the actual gameplay functionality through three methods, according
to the Model-View-Controller pattern:

```
while (running) 
{
  currentScene->Update(TIME_STEP);
  currentScene->HandleEvent(SDL_Event):
  currentScene->Render();
}
```

Our main game scene is called a Level, and hadles most of the gameplay through a Board -- the board contains a grid (columns x rows) of tile object entities
and handles the logic associated with it. So, a level will switch between states, during which animations are performed:
- Waiting for input
- Swapping tiles
- Clearing matches
- Gravitating tiles

The board will be called to check if moves are valid and to update the state of the grid itself:
```
if (board->CanSwap(selectedOne, selectedTwo))
{
  board->SwapTiles(selectedOne, selectedTwo);
  board->RemoveSelected();
  state = SWAPPING_TILES;
}
```

Player inputs are received by SDL and are handled before the update, activating the appropriate flags.


## ECS

To help me get a grip of how a simple ECS actually works, I based the main functionality of my ECS library on the example provided by [Austin Morlan](https://austinmorlan.com/posts/entity_component_system/).
It is by no means the most complete implementation, but provides a well-structured, easy to understand, basis. This architecture allows for very efficient memory calls,
since all components are stored in a compact array, which is iterated upon by systems.

Our entities are simple int identifiers:

```
using Entity = std::uint32_t;
```

Entities can be attributed components. Our components are simple structs containing data. For example:

```
struct EntityDestruction
{
    float timeLeft;
};
```

Systems are what hold most functionality. They act upon entities which have the required components.

```
// Destroys entity after delay
class DestroyEntitySystem : public System
{
public:
    void Init() override;
    bool Update(float dt);   
};
```

Besides systems, we have some classes, such as Board, which act as pseudo-systems as they hold functionality. 
The Board doesn't iterate upon every entity which has a set of components, but iterates only upon entities contained in its grid of tile objects -- when checking for matches, for example.




## Features
- [ ] GameOver screen
- [ ] Time trial -- get more time by doing combos / big matches
- [ ] Colorless tiles + Immovable tiles -- technically 99% implemented but not in the game
- [ ] Particle System to create entities with vector animations -- for animations such as explosions, conffetti, etc.
- [ ] Sound effects -- for clearing matches and activating abilities
- [x] Audio Manager -- plays music and sounds (no sounds implemented yet)
- [ ] Score pop-up indicators for each match cleared
- [x] Scoring system -- with combo modifier (not very noticeable without score pop-ups)
- [x] Dynamic text sprites -- e.g. score text 
- [x] Sprite based (looping, delayed) animations -- e.g. special tiles iddle animation
- [x] Special tiles activate special ability when cleared
- [x] Bigger matches create special tile objects (different types)
- [x] Vector based animations -- e.g. when tiles shrink or fall down
- [x] New tiles spawn and tiles drop down to fill empty spaces
- [x] Tiles disappear when matches are found
- [x] Player can swap tiles -- by clicking both tiles or dragging from one to the other
- [x] Dynamic grid -- 8x8 by default


## Resources

Background image was altered from <a href="https://www.freepik.com/free-vector/decorative-background-with-purple-damask-pattern_956821.htm">Image by kjpargeter</a> on Freepik

Tile sprites were created from <a href="https://www.freepik.com/free-vector/app-vector-icons-with-different-textures-cartoon-style-ui-stone_10602956.htm">Image by macrovector</a> on Freepik
