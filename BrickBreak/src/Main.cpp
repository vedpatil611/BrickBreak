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

Object* player;

int main()
{
    Window window(800, 800);

    Shader* shader = ResourceManager::loadShader("shader/vert.glsl", "shader/frag.glsl", "basic");
    ResourceManager::loadTexture("textures/Block.png", true, "block");
    ResourceManager::loadTexture("textures/SolidMetal.png", true, "solid_block");
    Texture* paddleTex = ResourceManager::loadTexture("textures/Paddle.png", true, "player");

    SpriteRenderer spriteRenderer(ResourceManager::shaders["basic"]);
  
    // Level one("levels/level1.lvl", window.getWidth(), window.getHeight() / 2);
    Level one("levels/level1.lvl", 800, 800 / 2);

    glm::vec2 playerSize(100.0f, 20.0f);
    glm::vec2 playerPos(
        800.0f / 2.0f - playerSize.x / 2,
        800.0f - playerSize.y
    );
    float velocity = 10.0f;
    player = new Object(playerPos, playerSize, paddleTex);

    while(!window.shouldClose())
    {
        window.clear();
        
        // glm::mat4 proj = glm::ortho(0.0f, (float) window.getWidth(), 0.0f, (float) window.getHeight(), -1.0f, 1.0f);
        glm::mat4 proj = glm::ortho(0.0f, 800.0f, 800.0f, 0.0f, -1.0f, 1.0f);
        shader->bind();
        shader->setUniformMat4("uProjection", proj);

        static int _i=0;_i++;int err=glGetError();if(err>0){printf("%d %d\n",_i,err);exit(1);}
        one.render(spriteRenderer);

        const auto& input = window.getInputs();
        if (input[GLFW_KEY_LEFT])
        {
            if (player->pos.x >= 0.0f) player->pos.x -= velocity;
        }
        else if (input[GLFW_KEY_RIGHT])
        {
            if (player->pos.x <= 800.0f - player->size.x) player->pos.x += velocity;
        }
        player->render(spriteRenderer);
    
        window.update();
    }

    delete player;
    ResourceManager::clear();

    return 0;
}
