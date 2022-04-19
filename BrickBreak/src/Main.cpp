#include "Game.h"
#include <chrono>
#include <memory>

int main()
{
    Game* game = Game::CreateGame();
    auto* window = game->getWindow();
 
    auto last = std::chrono::high_resolution_clock::now();

    while (!window->shouldClose()) 
    {
        auto now = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
        last = now;
        game->loop(deltaTime);
        // printf("%lf\n", deltaTime);
    }

    delete game;
    return 0;
}
