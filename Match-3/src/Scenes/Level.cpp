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

    /* -------------------------------- Register Systems -------------------------------- */

    // gravitySystem = ecsManager.RegisterSystem<GravitySystem>();
    renderSystem = ecsManager.RegisterSystem<RenderSystem>();
    clickTileSystem = ecsManager.RegisterSystem<ClickTileSystem>();
    // FIXME Don't allow systems to register without required components


    /* ------------------------------ Create Scene Objects ------------------------------ */

    // TODO pass this through a level manager?
    std::vector<TileColor> tileColors
    {
        TileColor::Black,
        TileColor::White,
        TileColor::Pink,
        TileColor::Blue,
        TileColor::Orange,
    };

    board = new Board(Vector2f{400.0f, 200.0f}, p_rows, p_cols, entityCreator);

    board->PopulateBoard(tileColors);

    state = WAITING_ONE;
}

void Level::HandleEvent(SDL_Event& event)
{
    switch(event.type) 
    {
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
                if (state == WAITING_ONE)
                {
                    Coordinates coords;
                    Vector2f mousePosition = Vector2f{float(event.button.x), float(event.button.y)};

                    if (clickTileSystem->ClickedEntity(mousePosition, coords))
                    {
                        tileOne = coords;

                        // FIXME remove print
                        std::cout << "Selecting tile [" << tileOne.x << " ," << tileOne.y << "]" << std::endl;
                    }

                    state = WAITING_TWO;
                }
                else if (state == WAITING_TWO)
                {
                    Coordinates coords;
                    Vector2f mousePosition = Vector2f{float(event.button.x), float(event.button.y)};

                    if (clickTileSystem->ClickedEntity(mousePosition, coords))
                    {
                        if (tileOne == coords)
                        {
                            state = WAITING_ONE;
                            // FIXME remove print
                            std::cout << "De-selecting tile [" << tileOne.x << " ," << tileOne.y << "]" << std::endl;
                        }

                        else if (Board::CanSwap(tileOne, coords))
                        {
                            tileTwo = coords;
                            board->SwapTiles(tileOne, tileTwo);

                            state = WAITING_ONE;

                            // FIXME remove print
                            std::cout << "Swapping tiles [" << tileOne.x << " ," << tileOne.y << "] and [" << tileTwo.x << " ," << tileTwo.y << "]" << std::endl;
                        }

                        else
                        {
                            tileOne = coords;

                            // FIXME remove print
                            std::cout << "Selecting tile [" << tileOne.x << " ," << tileOne.y << "]" << std::endl;
                        }
                    }
                    else
                    {
                        state = WAITING_ONE;
                        // FIXME remove print
                        std::cout << "De-selecting tile [" << tileOne.x << " ," << tileOne.y << "]" << std::endl;
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

}

void Level::Render()
{
    window.Clear();
    window.Render(background, Vector2f{0.0f, 0.0f}, Vector2f{1280.0f,720.0f});
    renderSystem->Update(window);
    window.Display();
}