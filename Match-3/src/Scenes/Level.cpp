#include "Scenes/Level.hpp"

#include <iostream>

extern ECS_Manager ecsManager;

Level::Level(RenderWindow& p_window, int p_rows, int p_cols, const char* p_background, const char* p_boardTexture)
    :window(p_window), rows(p_rows), cols(p_cols)
{

    /* ------------------------------- Setup Resources  ------------------------------- */

    windowDimensions = p_window.GetDimensions();

    background = window.LoadTexture(p_background);
    boardTexture = window.LoadTexture(p_boardTexture);
    scoreText = window.LoadTextureFromText("Score: ", SDL_Color{0, 0, 0});

    // Load Tile object textures
    entityCreator.LoadTextures(window);


    /* ------------------------------ Register Components ------------------------------ */
    
    ecsManager.RegisterComponent<Transform>();
    ecsManager.RegisterComponent<Sprite>();
    ecsManager.RegisterComponent<SpriteAnimation>();
    ecsManager.RegisterComponent<TileObject>();
    ecsManager.RegisterComponent<Swappable>();
    ecsManager.RegisterComponent<Translate>();
    ecsManager.RegisterComponent<Shrinking>();
    ecsManager.RegisterComponent<EntityDestruction>();
    ecsManager.RegisterComponent<ClearedTile>();
    ecsManager.RegisterComponent<Selected>();

    /* -------------------------------- Register Systems -------------------------------- */

    renderSystem = ecsManager.RegisterSystem<RenderSystem>();
    clickTileSystem = ecsManager.RegisterSystem<ClickTileSystem>();
    moveTileSystem = ecsManager.RegisterSystem<MoveTileSystem>();
    shrinkEntitySystem = ecsManager.RegisterSystem<ShrinkEntitySystem>();
    destroyEntitySystem = ecsManager.RegisterSystem<DestroyEntitySystem>();    
    clearTileSystem = ecsManager.RegisterSystem<ClearTileSystem>();    
    animateSelectedSystem = ecsManager.RegisterSystem<AnimateSelectedSystem>();
    animateSpriteSystem = ecsManager.RegisterSystem<AnimateSpriteSystem>();

    // FIXME Don't allow systems to register without required components


    /* ------------------------------ Create Scene Objects ------------------------------ */

    // TODO pass this through a level manager?
    tileColors = std::vector<TileColor>
    {
        // TileColor::Black,
        TileColor::Blue,
        TileColor::Green,
        TileColor::Magenta,
        TileColor::Purple,
        // TileColor::White,
        TileColor::Yellow,
        // TileColor::Colorless
    };

    board = new Board(Vector2f{360.0f, 360.0f}, p_rows, p_cols, entityCreator);

    board->PopulateBoard(tileColors);

    // FIXME remove
    // board->InsertTile(Coordinates{2,2}, TileColor::Yellow, TileType::Normal);
    // board->InsertTile(Coordinates{2,3}, TileColor::Yellow, TileType::Normal);
    // board->InsertTile(Coordinates{2,4}, TileColor::Yellow, TileType::Normal);
    // board->InsertTile(Coordinates{2,5}, TileColor::Yellow, TileType::Normal);
    // board->InsertTile(Coordinates{2,6}, TileColor::Yellow, TileType::Normal);

    // board->InsertTile(Coordinates{3,2}, TileColor::Magenta, TileType::Normal);
    // board->InsertTile(Coordinates{3,3}, TileColor::Magenta, TileType::Normal);
    // board->InsertTile(Coordinates{3,4}, TileColor::Magenta, TileType::Normal);
    // board->InsertTile(Coordinates{3,5}, TileColor::Magenta, TileType::Normal);
    // board->InsertTile(Coordinates{3,6}, TileColor::Magenta, TileType::Normal);


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
    destroyEntitySystem->Update(dt);
    animateSpriteSystem->Update(dt);
    animateSelectedSystem->Update(dt);
    shrinkEntitySystem->Update(dt);

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
                board->ResetScore();
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
}

void Level::Render()
{
    window.Clear();
    window.Render(background, Vector2f{0.0f, 0.0f}, Vector2f{0.0f, 0.0f}, Vector2f{720, 720}, windowDimensions);
    window.Render(boardTexture, board->GetBackgroundPosition(), Vector2f{0, 0}, Vector2f{1000, 1000}, board->GetBackgroundSize());
    window.Render(scoreText, Vector2f{30.0f, 10.0f});
    // window.Render(scoreText, Vector2f{50.0f, 10.0f}); // FIXME dynamic score value;
    renderSystem->Update(window);
    window.Display();
}