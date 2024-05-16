#include "main.hpp"
#include "game.hpp"


int main(int agrc, char* argv[])
{
    try 
    {
        Game::GetInstance()->Init();

        Game::GetInstance()->Loop();
    }
    catch(const std::runtime_error& ex) 
    {
        std::cerr << "Runtime error exception thrown: " << ex.what() << '\n';
        return 1;
    }
    catch(const std::invalid_argument& ex) 
    {
        std::cerr << "Invalid argument exception thrown: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}