#pragma once

#include "Ball.h"
#include "Level.h"
#include "Object.h"
#include "Window.h"
#include <string>
#include <vector>
#include <unordered_map>

class Game
{
public:
    inline static const float WIDTH = 800.0f;
    inline static const float HEIGHT = 600.0f;
  
    static Game* CreateGame();
    static void loop(float delta);

    inline static const Object* getPlayer() { return player; }
    inline static const Ball* getBall() { return ball; }
    inline static const Window* getWindow() { return window; }

    ~Game();
private:
    inline static Object* player = nullptr;
    inline static Ball* ball = nullptr;

    inline static Window* window = nullptr;
    inline static Game* gamePtr = nullptr;

    inline static std::unordered_map<std::string, SpriteRenderer*> renderers;
    inline static std::vector<Level*> levels;

    inline static int currentLevel = 0;

    Game();
};
