# Match-3

<p align="center"> <img src="https://github.com/TigoDelgado/Match-3/blob/master/Resources/match3gif.gif" width="400"/> </p>

## The challenge

I was challenged to develop a Match 3 game in C++ using the libSDL library. This is my first time actually working with SDL so it has been a great learning experience.
I also decided to take this oportunity to explore more data driven approaches, namely the ECS architecture - Entity Component System. 

## Core gameplay loop

Our game loop is comprised of two main loops: 
- waiting for the player to select two valid tiles to swap (yellow)
- clearing tile matches and spawning new tiles (blue)

![](https://github.com/TigoDelgado/Match-3/blob/master/Resources/Core%20gameplay%20loop.jpg)

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
It is by no means the most complete implementation, but provides a well-structured, easy to understand, basis. This architecture allows for very efficient memory calls, since all components are stored in a compact array, which is iterated upon by systems -- instead of having to recal entire objects into memory for example.

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

Entity behaviour is (almost) entirely defined by their components. Any entity containing a Transform component (includes position, rotation and scale) and a Sprite component (includes an SDL_Texture and dimensions) will be rendered by the RenderSystem in the appropriate location. This approach allows us to create a lot of modular fucntionality through components, making it easy to iterate upon the current program.

Our board has a grid of entities which are Tile Objects (meaning that they have a TileObject component) which are affected by different systems in the scene. 
After implementing normal tiles, it was pretty straightforward to implement special tiles with different behaviour. Any transformation we want to apply to entities,
like translate or scaling animations, can be done simply by adding a component to the entity, which will then be affected by the appropriate system. Now we can easily implement new tiles, such as colorless tiles (which can not be matched), or immovable tiles (similar to normal tiles but lacking the Swappable component).

Besides systems, we can also have methods which function as pseude-systems. The Board, for example, has integrated functionality which is applied not to every
entity with a set of components, but only to those entities held by the board's grid. The board will then add and remove components from its entities whenever
they are required to change.




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

### Tile Objects

**Basic Tiles** - Default tile objects with no special behaviour. They spawn randomly on the grid and are cleared when a match of three or more of the same colour is created.

<img src="https://github.com/TigoDelgado/Match-3/blob/master/Resources/all-tiles-normal.png" width="350"/>

**Vertical Tiles** - Special tiles that spawn when the player completes a vertical match of 4 tiles. When cleared, they clear their column in its entirety.

<img src="https://github.com/TigoDelgado/Match-3/blob/master/Resources/all-tiles-vertical.gif" width="350"/>


**Horizontal Tiles** - Special tiles that spawn when the player completes a horizontal match of 4 tiles. When cleared, they clear their row in its entirety.

<img src="https://github.com/TigoDelgado/Match-3/blob/master/Resources/all-tiles-horizontal.gif" width="350"/>



**Vortex Tiles** - Special tiles that spawn when the player completes a vertical or horizontal match of 5 tiles or more. When cleared, they clear all tiles of their colour.

<img src="https://github.com/TigoDelgado/Match-3/blob/master/Resources/all-tiles-vortex.gif" width="350"/>


**Exploding Tiles** - Special tiles that spawn when the player completes a mixed match - vertical and horizontal matches that meet. When cleared, they clear tiles in a 3x3 area around them.

<img src="https://github.com/TigoDelgado/Match-3/blob/master/Resources/all-tiles-explosion.gif" width="350"/>



**Nuclear Tiles** - Bigger exploding tiles that spawn when the player completes a mixed match - vertical and horizontal matches that meet - of 7 or more tiles. When cleared, they clear tiles in a 5x5 area around them.

<img src="https://github.com/TigoDelgado/Match-3/blob/master/Resources/all-tiles-big-explosion.gif" width="350"/>



## Resources

Background image was altered from <a href="https://www.freepik.com/free-vector/decorative-background-with-purple-damask-pattern_956821.htm">Image by kjpargeter</a> on Freepik

Tile sprites were created from <a href="https://www.freepik.com/free-vector/app-vector-icons-with-different-textures-cartoon-style-ui-stone_10602956.htm">Image by macrovector</a> on Freepik
