#include "Scenes/Level.hpp"

#include "ECS/Manager.hpp"
#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"
#include "Components/TileObject.hpp"
#include "Components/Swappable.hpp"
#include "Components/Translate.hpp"
#include "Components/Destruction.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

Level::Level(RenderWindow& p_window, int p_rows, int p_cols, const char* p_background)
    :window(p_window), rows(p_rows), cols(p_cols)
{

    /* ------------------------------- Setup Resources  ------------------------------- */

    background = window.LoadTexture(p_background);

    // Load Tile object textures
    entityCreator.LoadTextures(window);


    /* ------------------------------ Register Components ------------------------------ */
    
    ecsManager.RegisterComponent<Transform>();
    ecsManager.RegisterComponent<Sprite>();
    ecsManager.RegisterComponent<TileObject>();
    ecsManager.RegisterComponent<Swappable>();
    ecsManager.RegisterComponent<Translate>();
    ecsManager.RegisterComponent<Destruction>();

    /* -------------------------------- Register Systems -------------------------------- */

    // gravitySystem = ecsManager.RegisterSystem<GravitySystem>();
    renderSystem = ecsManager.RegisterSystem<RenderSystem>();
    clickTileSystem = ecsManager.RegisterSystem<ClickTileSystem>();
    moveTileSystem = ecsManager.RegisterSystem<MoveTileSystem>();
    clearTileSystem = ecsManager.RegisterSystem<ClearTileSystem>();

    // FIXME Don't allow systems to register without required components


    /* ------------------------------ Create Scene Objects ------------------------------ */

    // TODO pass this through a level manager?
    tileColors = std::vector<TileColor>
    {
        TileColor::Black,
        TileColor::White,
        TileColor::Pink,
        TileColor::Blue,
        TileColor::Orange,
    };

    board = new Board(Vector2f{400.0f, 200.0f}, p_rows, p_cols, entityCreator);

    board->PopulateBoard(tileColors);

    state = WAITING;
}

void Level::HandleEvent(SDL_Event& event)
{
    switch(event.type) 
    {
    case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            mouseDown = true;

            Vector2f mousePosition = Vector2f{float(event.button.x), float(event.button.y)};
            
            Entity entity;

            if (selectedOne == NULL_ENTITY) // Select first tile
            {
                if (clickTileSystem->ClickedTile(mousePosition, entity))
                {
                    selectedOne = entity;
                    // TODO add selected component?
                }
            }
            else // First tile already selected
            {
                if (clickTileSystem->ClickedTile(mousePosition, entity))
                {
                    if (entity == selectedOne)  // De-select tile
                    {
                        selectedOne = NULL_ENTITY;
                        selectedTwo = NULL_ENTITY;
                    }
                    else    
                    {
                        if (board->CanSwap(selectedOne, entity))     // Select second tile
                        {
                            selectedTwo = entity;
                        }
                        else    // Select new first tile
                        {
                            selectedOne = entity;
                        }
                    }
                }
            }
        }
        std::cout << "Mouse DOWN | Selected One: " << selectedOne << " | Selected Two: " << selectedTwo << std::endl;
        break;

    case SDL_MOUSEBUTTONUP:
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            mouseDown = false;

            if (selectedOne != NULL_ENTITY && selectedTwo == NULL_ENTITY)
            {
                Vector2f mousePosition = Vector2f{float(event.button.x), float(event.button.y)};
            
                Entity entity;

                if (clickTileSystem->ClickedTile(mousePosition, entity))
                {
                    if (selectedOne != entity)      // If mouse is dropped over other tile
                    {
                        selectedOne = NULL_ENTITY;
                        selectedTwo = NULL_ENTITY;
                    }
                }
                else                                // if mouse dropped somewhere else FIXME
                {
                    selectedOne = NULL_ENTITY;
                    selectedTwo = NULL_ENTITY;
                }
            }
        }
        std::cout << "Mouse UP | Selected One: " << selectedOne << " | Selected Two: " << selectedTwo << std::endl;
        break;

    case SDL_MOUSEMOTION:
        if (mouseDown && selectedOne != NULL_ENTITY && selectedTwo == NULL_ENTITY)  // Clicked tile and dragged to other tile
        {
            Vector2f mousePosition = Vector2f{float(event.button.x), float(event.button.y)};
            
            Entity entity;

            if (clickTileSystem->ClickedTile(mousePosition, entity))
                {
                    if (board->CanSwap(selectedOne, entity))     // Select second tile
                        {
                            selectedTwo = entity;
                        }
                }
        }

        break;

    default:
        break;
    }
}


void Level::Update(float dt)
{
    switch (state)
    {
    case WAITING:
        if (selectedOne != NULL_ENTITY && selectedTwo != NULL_ENTITY)
        {
            board->SwapTiles(selectedOne, selectedTwo);
            state = SWAPPING_TILES;
        }
        break;

    case SWAPPING_TILES:
        if (!moveTileSystem->Update(dt))
        {
            if (board->CheckMatches())
            {
                board->ClearMatches();
                selectedOne = NULL_ENTITY;
                selectedTwo = NULL_ENTITY;
                state = CLEARING_MATCHES;
            }
            else
            {
                board->SwapTiles(selectedOne, selectedTwo);
                state = SWAPPING_BACK;
            }
        }
        break;
    
    case SWAPPING_BACK:
        if (!moveTileSystem->Update(dt))
        {
            selectedOne = NULL_ENTITY;
            selectedTwo = NULL_ENTITY;
            state = WAITING;
        }
        break;

    case CLEARING_MATCHES:
        if (!clearTileSystem->Update(dt))
        {
            board->SpawnTiles(tileColors);
            
            state = GRAVITATING_ROWS;
        }
        break;

    case GRAVITATING_ROWS:
        if (!moveTileSystem->Update(dt))    // TODO create gravitate system?
        {
            if (board->CheckMatches())
            {
                board->ClearMatches();
                state = CLEARING_MATCHES;
            }
            else
            {
                state = WAITING;
            }
        }
        break;
    
    default:
        break;
    }
}

void Level::Render()
{
    window.Clear();
    window.Render(background, Vector2f{0.0f, 0.0f}, Vector2f{1280.0f,720.0f});
    renderSystem->Update(window);
    window.Display();
}