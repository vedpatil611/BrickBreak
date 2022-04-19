#include "Game.h"

#include "ResourceManager.h"
#include <glm/gtc/matrix_transform.hpp>

Game* Game::CreateGame()
{
    if (gamePtr) return gamePtr;
    gamePtr = new Game();
    return gamePtr;
}

void Game::loop(double delta)
{
    window->clear();
    SpriteRenderer* spriteRenderer = renderers["basic"];
    
    if (!levels[currentLevel]->isLoaded()) levels[currentLevel]->load();
    levels[currentLevel]->render(*spriteRenderer);

    Game::processInput(delta);
    
    player->render(*spriteRenderer);
 
    ball->move(delta, WIDTH);
    ball->render(*spriteRenderer);

    Game::processCollision();

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
    levels[0]->load();

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

    const glm::vec2 ballVelocity(0.7f, -0.7f);
    const float BALL_RADIUS = 12.0f;
    glm::vec2 ballPos = playerPos + glm::vec2(playerSize.x / 2 - BALL_RADIUS, - BALL_RADIUS * 2.0f);
    ball = new Ball(ballPos, BALL_RADIUS, ballVelocity, ballTex);
}

void Game::processInput(double delta)
{
    const auto& input = window->getInputs();
    float velocity = 1.0f;

    if (input[GLFW_KEY_LEFT])
    {
        if (player->pos.x >= 0.0f) 
        {
            player->pos.x -= velocity * delta;
            if (ball->stuck) ball->pos.x -= velocity * delta;
        }
    }
    else if (input[GLFW_KEY_RIGHT])
    {
        if (player->pos.x <= WIDTH - player->size.x) 
        {
            player->pos.x += velocity * delta;
            if (ball->stuck) ball->pos.x += velocity * delta;
        }
    }

    if (input[GLFW_KEY_SPACE])
    {
        ball->stuck = false;
    }
}

bool Game::checkCollision(Object* one, Object* two)
{
    bool x = one->pos.x + one->size.x >= two->pos.x && two->pos.x + two->size.x >= one->pos.x;
    bool y = one->pos.y + one->size.y >= two->pos.y && two->pos.y + two->size.y >= one->pos.y;

    return x && y;
}
    
bool Game::checkCollision(Ball* ball, Object* obj)
{
    glm::vec2 center(ball->pos + ball->radius);
    glm::vec2 aabbHalfExtent(obj->size.x / 2.0f, obj->size.y / 2.0f);
    glm::vec2 aabbCenter(
        obj->pos.x + aabbHalfExtent.x,
        obj->pos.y + aabbHalfExtent.y
    );

    glm::vec2 diff = center - aabbCenter;
    glm::vec2 clamped = glm::clamp(diff, -aabbHalfExtent, aabbHalfExtent);
    
    glm::vec2 closest = aabbCenter + clamped;
    diff = closest - center;

    return glm::length(diff) < ball->radius;
}

void Game::processCollision()
{
    for(Object& box: levels[currentLevel]->objects)
    {
        if(!box.destroyed)
        {
            if (checkCollision(ball, &box))
            {
                if(!box.isSolid) 
                {
                    box.destroyed = true;
                }
            }
        }
    }
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
