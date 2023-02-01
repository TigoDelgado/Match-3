#include "Scenes/MainMenu.hpp"
#include "Scenes/Level.hpp"

#include <string>

extern ECS_Manager ecsManager;
extern SceneManager sceneManager;
extern AudioManager audioManager;
extern EntityCreator entityCreator;

MainMenu::MainMenu(RenderWindow& p_window)
    :window(p_window)
{
    windowDimensions = p_window.GetDimensions();

    renderSystem = sceneManager.renderSystem;
    clickButtonSystem = sceneManager.clickButtonSystem;
    updateTextSystem = sceneManager.updateTextSystem;

    background = window.LoadTexture(BACKGROUND_PATH);

    buttons.push_back(entityCreator.CreateButtonEntity(0, ButtonType::START, Vector2f{100,50}, Vector2f{360,460}));
    // buttons.push_back(entityCreator.CreateButtonEntity(1, ButtonType::QUIT, Vector2f{100,50}, Vector2f{420,450}));

    buttons.push_back(entityCreator.CreateButtonEntity(2, ButtonType::MINUS, Vector2f{50,50}, Vector2f{420,260}));    // cols--
    buttons.push_back(entityCreator.CreateButtonEntity(3, ButtonType::PLUS, Vector2f{50,50}, Vector2f{500,260}));    // cols++

    buttons.push_back(entityCreator.CreateButtonEntity(4, ButtonType::MINUS, Vector2f{50,50}, Vector2f{420,320}));    // rows--
    buttons.push_back(entityCreator.CreateButtonEntity(5, ButtonType::PLUS, Vector2f{50,50}, Vector2f{500,320}));    // rows++

    buttons.push_back(entityCreator.CreateButtonEntity(6, ButtonType::MINUS, Vector2f{50,50}, Vector2f{420,380}));    // colors--
    buttons.push_back(entityCreator.CreateButtonEntity(7, ButtonType::PLUS, Vector2f{50,50}, Vector2f{500,380}));    // colors++

    colsText = (entityCreator.CreateTextEntity("8", Vector2f{340, 260}, SDL_Color{10,20,30}, window));
    rowsText = (entityCreator.CreateTextEntity("8", Vector2f{340, 320}, SDL_Color{10,20,30}, window));
    colorsText = (entityCreator.CreateTextEntity("5", Vector2f{340, 380}, SDL_Color{10,20,30}, window));

    buttons.push_back(entityCreator.CreateTextEntity("Columns:  ", Vector2f{240, 260}, SDL_Color{10,20,30}, window));
    buttons.push_back(entityCreator.CreateTextEntity("Rows:     ", Vector2f{290, 320}, SDL_Color{10,20,30}, window));
    buttons.push_back(entityCreator.CreateTextEntity("Colors:   ", Vector2f{265, 380}, SDL_Color{10,20,30}, window));

    buttonClicked = -1;
}

void MainMenu::HandleEvent(SDL_Event& event)
{
switch(event.type) 
    {
    case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            Vector2f mousePosition = Vector2f{float(event.button.x), float(event.button.y)};
            buttonClicked = clickButtonSystem->ClickedButton(mousePosition);
        }
        break;
    }
}


void MainMenu::Update(float dt)
{
    if (buttonClicked != -1)
    {
        if (buttonClicked == 0)
        {
            // START LEVEL
            startLevel = true;
        }
        else if (buttonClicked == 1)
        {
            // QUIT GAME
        }
        else if (buttonClicked == 2)
        {
            // Cols--
            if (cols > 5)
            {
                cols--;
                ecsManager.AddComponent<Text>(colsText, Text{std::to_string(cols).c_str(), SDL_Color{10,20,30}});
            }
        }
        else if (buttonClicked == 3)
        {
            // Cols++
            if (cols < 10)
            {
                cols++;
                ecsManager.AddComponent<Text>(colsText, Text{std::to_string(cols).c_str(), SDL_Color{10,20,30}});
            }
        }
        else if (buttonClicked == 4)
        {
            // Rows--
            if (rows > 5)
            {
                rows--;
                ecsManager.AddComponent<Text>(rowsText, Text{std::to_string(rows).c_str(), SDL_Color{10,20,30}});
            }
        }
        else if (buttonClicked == 5)
        {
            // Rows++
            if (rows < 10)
            {
                rows++;
                ecsManager.AddComponent<Text>(rowsText, Text{std::to_string(rows).c_str(), SDL_Color{10,20,30}});
            }
        }
        else if (buttonClicked == 6)
        {
            // Colors--
            if (colors > 3)
            {
                colors--;
                ecsManager.AddComponent<Text>(colorsText, Text{std::to_string(colors).c_str(), SDL_Color{10,20,30}});
            }
        }
        else if (buttonClicked == 7)
        {
            // Colors++
            if (colors < 6)
            {
                colors++;
                ecsManager.AddComponent<Text>(colorsText, Text{std::to_string(colors).c_str(), SDL_Color{10,20,30}});
            }
        }
        buttonClicked = -1;
    }

    updateTextSystem->Update(window);
}


void MainMenu::Render()
{
    window.Clear();
    window.Render(background, Vector2f{0.0f, 0.0f}, Vector2f{0.0f, 0.0f}, Vector2f{720, 720}, windowDimensions);
    renderSystem->Update(window);
    window.Display();
}

bool MainMenu::ChangeScene()
{
    return startLevel;
}

GameScene* MainMenu::GetNextScene()
{
    return new Level(window, cols, rows, colors, "../res/Background/bs7.png", "../res/Board.png");
}

MainMenu::~MainMenu()
{
    // std::cout << "MY DOOM WAS ORDERED!" << std::endl;
    for (Entity button : buttons)
    {
        ecsManager.DestroyEntity(button);
    }
    ecsManager.DestroyEntity(colsText);
    ecsManager.DestroyEntity(rowsText);
    ecsManager.DestroyEntity(colorsText);

    SDL_DestroyTexture(background);
}