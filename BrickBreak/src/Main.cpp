#include "Ball.h"
#include "Level.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "SpriteRenderer.h"
#include "Window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <unistd.h>

enum GameState {
    PAUSED,
    ACTIVE,
};

const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

Object* player;
Ball* ball;

int main()
{
    Window window(800, 600);

    Shader* shader = ResourceManager::loadShader("shader/vert.glsl", "shader/frag.glsl", "basic");
    ResourceManager::loadTexture("textures/Block.png", true, "block");
    ResourceManager::loadTexture("textures/SolidMetal.png", true, "solid_block");
    Texture* paddleTex = ResourceManager::loadTexture("textures/Paddle.png", true, "player");
    Texture* ballTex = ResourceManager::loadTexture("textures/Ball.png", true, "ball");
        
    SpriteRenderer spriteRenderer(ResourceManager::shaders["basic"]);
  
    // Level one("levels/level1.lvl", window.getWidth(), window.getHeight() / 2);
    Level one("levels/level1.lvl", WIDTH, HEIGHT / 2);

    glm::vec2 playerSize(100.0f, 20.0f);
    glm::vec2 playerPos(
        WIDTH / 2.0f - playerSize.x / 2,
        HEIGHT - playerSize.y
    );
    float velocity = 10.0f;
    player = new Object(playerPos, playerSize, paddleTex);

    const glm::vec2 ballVelocity(12.0f, 12.0f);
    const float BALL_RADIUS = 12.0f;
    glm::vec2 ballPos = playerPos + glm::vec2(playerSize.x / 2 - BALL_RADIUS, - BALL_RADIUS * 2.0f);
    ball = new Ball(ballPos, BALL_RADIUS, ballVelocity, ballTex);

    while(!window.shouldClose())
    {
        window.clear();
        
        // glm::mat4 proj = glm::ortho(0.0f, (float) window.getWidth(), 0.0f, (float) window.getHeight(), -1.0f, 1.0f);
        glm::mat4 proj = glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, -1.0f, 1.0f);
        shader->bind();
        shader->setUniformMat4("uProjection", proj);

        static int _i=0;_i++;int err=glGetError();if(err>0){printf("%d %d\n",_i,err);exit(1);}
        one.render(spriteRenderer);

        const auto& input = window.getInputs();
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
        player->render(spriteRenderer);
 
        ball->move(0, WIDTH);
        ball->render(spriteRenderer);

        window.update();
    }

    delete player;
    ResourceManager::clear();

    return 0;
}
