#include "Scenes/Level.hpp"

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
    ecsManager.RegisterComponent<Selected>();

    /* -------------------------------- Register Systems -------------------------------- */

    // gravitySystem = ecsManager.RegisterSystem<GravitySystem>();
    renderSystem = ecsManager.RegisterSystem<RenderSystem>();
    clickTileSystem = ecsManager.RegisterSystem<ClickTileSystem>();
    moveTileSystem = ecsManager.RegisterSystem<MoveTileSystem>();
    clearTileSystem = ecsManager.RegisterSystem<ClearTileSystem>();
    animateSelectedSystem = ecsManager.RegisterSystem<AnimateSelectedSystem>();

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
    blocked = false;
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

            if (!blocked && selectedOne == NULL_ENTITY) 
            {
                if (clickTileSystem->ClickedTile(mousePosition, entity))    // Select first tile
                {
                    selectedOne = entity;
                    changedSelectedOne = true;
                }
            }
            else if (!blocked) // First tile already selected
            {
                if (clickTileSystem->ClickedTile(mousePosition, entity))
                {
                    if (entity == selectedOne)  // De-select tile
                    {
                        lastSelected = selectedOne;
                        selectedOne = NULL_ENTITY;
                        selectedTwo = NULL_ENTITY;

                        changedSelectedOne = true;
                    }
                    else    
                    {
                        if (board->CanSwap(selectedOne, entity))    // Select second tile
                        {
                            blocked = true;                         // Assures only one move queued while in clearing loop
                            selectedTwo = entity;
                            changedSelectedTwo = true;
                        }
                        else    // Select new first tile
                        {
                            lastSelected = selectedOne;
                            selectedOne = entity;

                            changedSelectedOne = true;
                        }
                    }
                }
                else   // if clicked outside grid, de-select current tile
                {
                    lastSelected = selectedOne;
                    selectedOne = NULL_ENTITY;
                    selectedTwo = NULL_ENTITY;

                    changedSelectedOne = true;
                }
            }
        }
        // std::cout << "Mouse DOWN | Selected One: " << selectedOne << " | Selected Two: " << selectedTwo << std::endl;
        break;

    case SDL_MOUSEBUTTONUP:
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            mouseDown = false;
        }
        // std::cout << "Mouse UP | Selected One: " << selectedOne << " | Selected Two: " << selectedTwo << std::endl;
        break;

    case SDL_MOUSEMOTION:
        if (!blocked && mouseDown && selectedOne != NULL_ENTITY && selectedTwo == NULL_ENTITY)  // Clicked tile and dragged to other tile
        {
            Vector2f mousePosition = Vector2f{float(event.button.x), float(event.button.y)};
            
            Entity entity;

            if (clickTileSystem->ClickedTile(mousePosition, entity))
                {
                    if (board->CanSwap(selectedOne, entity))     // Select second tile
                        {
                            blocked = true;
                            selectedTwo = entity;
                            changedSelectedTwo = true;
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
    if (changedSelectedOne)
    {
        // std::cout << "Changed selected from " << lastSelected  << " to " << selectedOne << std::endl;
        if (lastSelected != NULL_ENTITY)
        {
            // std::cout << "Removing Selected to entity " << lastSelected << std::endl;
            board->RemoveSelected(lastSelected);
        }
        if (selectedOne != NULL_ENTITY)
        {
            // std::cout << "Adding Selected to entity " << selectedOne << std::endl;
            board->AddSelected(selectedOne);
        }
        changedSelectedOne = false;
    }
    if (changedSelectedTwo)
    {
        if (selectedTwo != NULL_ENTITY)
        {
            // std::cout << "Adding Selected to second entity " << selectedOne << std::endl;
            board->AddSelected(selectedTwo);
        }
        changedSelectedTwo = false;
    }

    switch (state)
    {
    case WAITING:

        if (selectedOne != NULL_ENTITY && selectedTwo != NULL_ENTITY)
        {
            if (board->CanSwap(selectedOne, selectedTwo))
            {
                board->SwapTiles(selectedOne, selectedTwo);
                board->RemoveSelected(selectedOne);
                board->RemoveSelected(selectedTwo);
                state = SWAPPING_TILES;
                blocked = true;
            }
            else    // Prevents invalid moves selected during clearance loop
            {
                std::cout << "I PREVENTED AN INVALID MOVE" << std::endl;
                board->RemoveSelected(selectedTwo);
                selectedTwo = NULL_ENTITY;
            }
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
                blocked = false;
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
            blocked = false;
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
                blocked = false;
            }
        }
        break;
    
    default:
        break;
    }

    animateSelectedSystem->Update(dt);
}

void Level::Render()
{
    window.Clear();
    window.Render(background, Vector2f{0.0f, 0.0f}, Vector2f{1280.0f,720.0f}, 0, NULL, SDL_FLIP_NONE);
    renderSystem->Update(window);
    window.Display();
}