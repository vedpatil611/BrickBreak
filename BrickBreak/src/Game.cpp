#include "Game.h"

#include "ResourceManager.h"
#include <glm/gtc/matrix_transform.hpp>

Game* Game::CreateGame()
{
    if (gamePtr) return gamePtr;
    gamePtr = new Game();
    return gamePtr;
}

void Game::loop(float delta)
{
    window->clear();
    SpriteRenderer* spriteRenderer = renderers["basic"];
    float velocity = 10.0f;
    
    if (!levels[currentLevel]->isLoaded()) levels[currentLevel]->load();
    levels[currentLevel]->render(*spriteRenderer);

    const auto& input = window->getInputs();
    if (input[GLFW_KEY_LEFT])
    {
        if (player->pos.x >= 0.0f) 
        {
            player->pos.x -= velocity;
            if (ball->stuck) ball->pos.x -= velocity;
        }
    }
    else if (input[GLFW_KEY_RIGHT])
    {
        if (player->pos.x <= WIDTH - player->size.x) 
        {
            player->pos.x += velocity;
            if (ball->stuck) ball->pos.x += velocity;
        }
    }
    player->render(*spriteRenderer);
 
    ball->move(0, WIDTH);
    ball->render(*spriteRenderer);

    window->update();
}

Game::Game()
{
    gamePtr = this;
    
    window = new Window(800.0f, 600.0f);
 
    Shader* shader = ResourceManager::loadShader("shader/vert.glsl", "shader/frag.glsl", "basic");
    ResourceManager::loadTexture("textures/Block.png", true, "block");
    ResourceManager::loadTexture("textures/SolidMetal.png", true, "solid_block");
    Texture* paddleTex = ResourceManager::loadTexture("textures/Paddle.png", true, "player");
    Texture* ballTex = ResourceManager::loadTexture("textures/Ball.png", true, "ball");

    SpriteRenderer* spriteRenderer = new SpriteRenderer(ResourceManager::shaders["basic"]);
    renderers["basic"] = spriteRenderer;

    levels.push_back(new Level("levels/level1.lvl", WIDTH, HEIGHT / 2));

    glm::mat4 proj = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, -1.0f, 1.0f);
    shader->bind();
    shader->setUniformMat4("uProjection", proj);

    glm::vec2 playerSize(100.0f, 20.0f);
    glm::vec2 playerPos(
        WIDTH / 2.0f - playerSize.x / 2,
        HEIGHT - playerSize.y
    );
    // float velocity = 10.0f;
    player = new Object(playerPos, playerSize, paddleTex);

    const glm::vec2 ballVelocity(12.0f, 12.0f);
    const float BALL_RADIUS = 12.0f;
    glm::vec2 ballPos = playerPos + glm::vec2(playerSize.x / 2 - BALL_RADIUS, - BALL_RADIUS * 2.0f);
    ball = new Ball(ballPos, BALL_RADIUS, ballVelocity, ballTex);
}

Game::~Game()
{
    delete player;
    delete ball;
    for (auto& [k, v]: renderers) delete v;
    for (auto l: levels) delete l;
    ResourceManager::clear();
    delete window;
}
