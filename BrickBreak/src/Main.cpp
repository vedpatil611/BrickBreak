#include "Game.h"
#include <memory>

int main()
{
    Game* game = Game::CreateGame();
    while (!game->getWindow()->shouldClose()) game->loop(0.0f);
    delete game;
    return 0;
}
