#include "Game.h"

#include "BatchRenderer.h"
#include "Particle.h"
#include "ResourceManager.h"
#include <glm/gtc/matrix_transform.hpp>

Game* Game::CreateGame()
{
    if (gamePtr) return gamePtr;
    gamePtr = new Game();
    return gamePtr;
}

Game::Game()
{
    gamePtr = this;
    
    window = new Window(800.0f, 600.0f);
 
    // Shader* shader = ResourceManager::loadShader("shader/vert.glsl", "shader/frag.glsl", "basic");
    Shader* shader = ResourceManager::loadShader("shader/batch_vert.glsl", "shader/batch_frag.glsl", "batch");
    Shader* particleShader = ResourceManager::loadShader("shader/particle_vert.glsl", "shader/particle_frag.glsl", "particle");
    ResourceManager::loadTexture("textures/Block.png", true, "block");
    ResourceManager::loadTexture("textures/SolidMetal.png", true, "solid_block");
    Texture* paddleTex = ResourceManager::loadTexture("textures/Paddle.png", true, "player");
    Texture* ballTex = ResourceManager::loadTexture("textures/Ball.png", true, "ball");
    Texture* particleTex = ResourceManager::loadTexture("textures/Particle.png", true, "particle");

    // SpriteRenderer* spriteRenderer = new SpriteRenderer(ResourceManager::shaders["basic"]);
    BatchRenderer* batchRenderer = new BatchRenderer(ResourceManager::shaders["batch"]);

    // renderers["basic"] = spriteRenderer;
    renderers["batch"] = batchRenderer;

    levels.push_back(new Level("levels/level1.lvl", WIDTH, HEIGHT / 2));
    levels[0]->load();

    glm::mat4 proj = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, -2.0f, 2.0f);
    shader->bind();
    shader->setUniformMat4("uProjection", proj);
    particleShader->bind();
    particleShader->setUniformMat4("uProjection", proj);
    particleShader->unbind();

    // Particle particleInstance = Particle();

    Game::particleGenerator = new ParticleGenerator(particleShader, particleTex, 500);

    glm::vec2 playerSize(100.0f, 20.0f);
    glm::vec2 playerPos(
        WIDTH / 2.0f - playerSize.x / 2,
        HEIGHT - playerSize.y
    );
    // float velocity = 10.0f;
    player = new Object(playerPos, playerSize, paddleTex);

    const glm::vec2 ballVelocity(300.0f, -300.0f);
    const float BALL_RADIUS = 12.0f;
    glm::vec2 ballPos = playerPos + glm::vec2(playerSize.x / 2 - BALL_RADIUS, - BALL_RADIUS * 2.0f);
    ball = new Ball(ballPos, BALL_RADIUS, ballVelocity, ballTex);
}

void Game::loop(double delta)
{
    window->clear();
    // SpriteRenderer* spriteRenderer = (SpriteRenderer*) renderers["basic"];
    BatchRenderer* renderer = (BatchRenderer*) renderers["batch"];

    if (!levels[currentLevel]->isLoaded()) levels[currentLevel]->load();
    
    particleGenerator->update(delta, ball, 1, glm::vec2(ball->radius / 2.0f));
    particleGenerator->draw();
    // if(GLenum err=glGetError()){printf("%d\n",err);exit(0);}

    renderer->begin();
    levels[currentLevel]->render(renderer);

    Game::processInput(delta);
    
    renderer->submit(player);
    // player->render(spriteRenderer);
 
    ball->move(delta, WIDTH);
    renderer->submit(ball);
    // ball->render(spriteRenderer);
    
    renderer->end();
    renderer->render();

    Game::processCollision();

    if (ball->pos.y >= HEIGHT) 
    {
        player->pos = glm::vec2(WIDTH / 2.0f - player->size.x / 2.0f, HEIGHT - player->size.y);
        ball->reset(player->pos + glm::vec2(player->size.x / 2.0f - ball->radius, - ball->radius * 2.0f));
    }

    window->update();
}

void Game::processInput(double delta)
{
    float dt = static_cast<float>(delta);
    const auto& input = window->getInputs();
    float velocity = 500.0f;

    if (input[GLFW_KEY_LEFT])
    {
        if (player->pos.x >= 0.0f) 
        {
            player->pos.x -= velocity * dt;
            if (ball->stuck) ball->pos.x -= velocity * dt;
        }
    }
    else if (input[GLFW_KEY_RIGHT])
    {
        if (player->pos.x <= WIDTH - player->size.x) 
        {
            player->pos.x += velocity * dt;
            if (ball->stuck) ball->pos.x += velocity * dt;
        }
    }

    if (input[GLFW_KEY_SPACE])
    {
        ball->stuck = false;
    }
}
   
Collision Game::checkCollision(Ball* ball, Object* obj)
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

    if (glm::length(diff) <= ball->radius)
        return std::make_tuple(true, VectorDirection(diff), diff);
    else
        return std::make_tuple(false, Direction::UP, glm::vec2(0.0f, 0.0f));
}

void Game::processCollision()
{
    for(Object& box: levels[currentLevel]->objects)
    {
        if(!box.destroyed)
        {
            Collision collision = checkCollision(ball, &box);
            if (std::get<0>(collision))
            {
                if(!box.isSolid) box.destroyed = true;

                Direction dir = std::get<1>(collision);
                glm::vec2 diff = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT)
                {
                    ball->velocity.x *= -1.0f;
                    
                    float penetration = ball->radius - std::abs(diff.x);
                    if(dir == LEFT)
                        ball->pos.x += penetration;
                    else
                        ball->pos.x -= penetration;
                }
                else
                {
                    ball->velocity.y *= -1.0f;
                    
                    float penetration = ball->radius - std::abs(diff.y);
                    if(dir == UP)
                        ball->pos.y -= penetration;
                    else
                        ball->pos.y += penetration;
                }
            }
        }
    }

    Collision result = checkCollision(ball, player);
    if(!ball->stuck && std::get<0>(result))
    {
        float boardCenter = player->pos.x + player->size.x / 2.0f;
        float distance = (ball->pos.x + ball->radius) - boardCenter;
        float ratio = distance / (player->size.x / 2.0f);

        float strenght = 2.0f;
        glm::vec2 oldVelocity = ball->velocity;
        ball->velocity.x = ball->initialVelocity.x * ratio * strenght;
        // ball->velocity.x = ball->velocity.x * ratio * strenght;
        ball->velocity.y = -1.0f * std::abs(ball->velocity.y);
        
        ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
    }
}

Game::~Game()
{
    delete player;
    delete ball;
    for (auto& [k, v]: renderers) delete v;
    for (auto l: levels) delete l;
    delete particleGenerator;
    ResourceManager::clear();
    delete window;
}
