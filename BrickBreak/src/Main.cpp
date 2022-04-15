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

int main()
{
    Window window(600, 600);

    Shader* shader = ResourceManager::loadShader("shader/vert.glsl", "shader/frag.glsl", "basic");
    ResourceManager::loadTexture("textures/Block.png", true, "block");
    ResourceManager::loadTexture("textures/SolidMetal.png", true, "solid_block");

    SpriteRenderer spriteRenderer(ResourceManager::shaders["basic"]);
  
    // Level one("levels/level1.lvl", window.getWidth(), window.getHeight() / 2);
    Level one("levels/level1.lvl", 800, 800 / 2);

    while(!window.shouldClose())
    {
        window.clear();
        
        // glm::mat4 proj = glm::ortho(0.0f, (float) window.getWidth(), 0.0f, (float) window.getHeight(), -1.0f, 1.0f);
        glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f);
        shader->bind();
        shader->setUniformMat4("uProjection", proj);

        static int _i=0;_i++;int err=glGetError();if(err>0){printf("%d %d\n",_i,err);exit(1);}
        one.render(spriteRenderer);
    
        window.update();
    }

    ResourceManager::clear();

    return 0;
}
