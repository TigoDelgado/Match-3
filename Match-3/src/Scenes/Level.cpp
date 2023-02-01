#include "Scenes/Level.hpp"

#include <iostream>
#include <string>

extern ECS_Manager ecsManager;
extern SceneManager sceneManager;
extern AudioManager audioManager;
extern EntityCreator entityCreator;

Level::Level(RenderWindow& p_window, int p_rows, int p_cols, int p_colors, const char* p_background, const char* p_boardTexture)
    :window(p_window), rows(p_rows), cols(p_cols)
{

    /* ------------------------------- Setup Resources  ------------------------------- */

    windowDimensions = p_window.GetDimensions();

    background = window.LoadTexture(p_background);
    boardTexture = window.LoadTexture(p_boardTexture);

    // scoreText = window.LoadTextureFromText("Score: ", SDL_Color{255, 255, 255});


    /* ------------------------------- Get Scene Systems  ------------------------------- */

    renderSystem = sceneManager.renderSystem;
    clickTileSystem = sceneManager.clickTileSystem;
    moveTileSystem = sceneManager.moveTileSystem;
    expandSystem = sceneManager.expandSystem;
    destroyEntitySystem = sceneManager.destroyEntitySystem;
    clearTileSystem = sceneManager.clearTileSystem;
    animateSelectedSystem = sceneManager.animateSelectedSystem;
    animateSpriteSystem = sceneManager.animateSpriteSystem;
    delayAnimationSystem = sceneManager.delayAnimationSystem;
    updateTextSystem = sceneManager.updateTextSystem;
    clickButtonSystem = sceneManager.clickButtonSystem;

    /* ------------------------------ Create Scene Objects ------------------------------ */

    tileColors.push_back(TileColor::Purple);
    tileColors.push_back(TileColor::Yellow);
    tileColors.push_back(TileColor::Blue);
    if (p_colors > 3) tileColors.push_back(TileColor::Magenta);
    if (p_colors > 4) tileColors.push_back(TileColor::Green);
    if (p_colors > 5) tileColors.push_back(TileColor::Black);

    board = new Board(Vector2f{360.0f, 340.0f}, p_rows, p_cols, entityCreator);

    board->PopulateBoard(tileColors);

    Vector2f scorePosition = Vector2f{360.0f, 340.0f};
    scorePosition.y += board->GetBackgroundSize().y / 2 + 40;
    scoreText = entityCreator.CreateTextEntity("000", scorePosition, SDL_Color{255, 225, 240}, window);

    backButton = entityCreator.CreateButtonEntity(0, ButtonType::BACK, Vector2f{50,50}, Vector2f{670,670});

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
            buttonClicked = clickButtonSystem->ClickedButton(mousePosition);

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
    delayAnimationSystem->Update(dt);
    destroyEntitySystem->Update(dt);
    animateSpriteSystem->Update(dt);
    animateSelectedSystem->Update(dt);
    expandSystem->Update(dt);

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
                score += addScore;
                board->ClearMatches();
                addScore = board->GetScore();
                ecsManager.AddComponent<Text>(scoreText, Text{std::to_string(score + addScore).c_str(), SDL_Color{255,225,230}});
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
                addScore = board->GetScore();
                ecsManager.AddComponent<Text>(scoreText, Text{std::to_string(score + addScore).c_str(), SDL_Color{255,225,230}});
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

    if( Mix_PlayingMusic() == 0 )
    {
        audioManager.PlayRandomSoundtrack(0);
    }

    updateTextSystem->Update(window);

    if (buttonClicked != -1)
    {
        if (buttonClicked == 0)
        {
            // BACK TO MENU
            backToMenu = true;
        }
    }
}

void Level::Render()
{
    window.Clear();
    window.Render(background, Vector2f{0.0f, 0.0f}, Vector2f{0.0f, 0.0f}, Vector2f{720, 720}, windowDimensions);
    window.Render(boardTexture, board->GetBackgroundPosition(), Vector2f{0, 0}, Vector2f{1000, 1000}, board->GetBackgroundSize());
    renderSystem->Update(window);
    window.Display();
}

bool Level::ChangeScene()
{
    return backToMenu;
}

GameScene* Level::GetNextScene()
{
    return new MainMenu(window);
}

Level::~Level()
{
    Mix_HaltMusic();
    board->~Board();
    ecsManager.DestroyEntity(backButton);
    ecsManager.DestroyEntity(scoreText);
}