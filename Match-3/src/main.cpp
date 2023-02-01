#include <iostream>
#include "Game.hpp"

int main(int argc, char* argv[]) 
{
    Game game;

    try {
        game.Run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}