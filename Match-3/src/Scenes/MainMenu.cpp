#include "Scenes/MainMenu.hpp"
#include "Scenes/Level.hpp"

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

    background = window.LoadTexture(BACKGROUND_PATH);

    buttons.push_back(entityCreator.CreateButtonEntity(0, ButtonType::START, Vector2f{100,50}, Vector2f{260,450}));
    buttons.push_back(entityCreator.CreateButtonEntity(1, ButtonType::QUIT, Vector2f{100,50}, Vector2f{460,450}));
    buttons.push_back(entityCreator.CreateButtonEntity(2, ButtonType::DEFAULT, Vector2f{50,50}, Vector2f{360,450}));

    buttonClicked = -1;

    audioManager.PlayRandomSoundtrack();
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
            std::cout << "START LEVEL" << std::endl;
            startLevel = true;
        }
        else if (buttonClicked == 1)
        {
            // QUIT GAME
            std::cout << "QUIT GAME" << std::endl;
        }
        else if (buttonClicked == 2)
        {
            // DO STUFF
            std::cout << "do stuff?" << std::endl;
        }
        else if (buttonClicked == 3)
        {
            
        }
        else if (buttonClicked == 4)
        {
            
        }
        else if (buttonClicked == 5)
        {
            
        }
        else if (buttonClicked == 6)
        {
            
        }
        else if (buttonClicked == 7)
        {
            
        }
        buttonClicked = -1;
    }
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
    return new Level(window, 8, 8, "../res/Background/bs7.png", "../res/Board.png");
}

MainMenu::~MainMenu()
{
    std::cout << "MY DOOM WAS ORDERED!" << std::endl;
    for (Entity button : buttons)
    {
        ecsManager.DestroyEntity(button);
    }
}