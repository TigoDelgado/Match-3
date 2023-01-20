#include <iostream>
#include "Game.hpp"

int main(int argc, char* argv[]) 
{
    Game game;

    try {
        game.Run();
        //TODO create and handle exceptions
    } catch (...) {
        std::cerr << "An unhandled exception was thrown" << std::endl;
        return 1;
    }
    return 0;
}