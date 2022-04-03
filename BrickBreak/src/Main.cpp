#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Window.h"

int main()
{
    Window window(600, 600);

    ResourceManager::loadShader("shader/vert.glsl", "shader/frag.glsl", "basic");
    ResourceManager::loadTexture("textures/Block.png", true, "block");
    ResourceManager::loadTexture("textures/SolidMetal.png", true, "solid_block");

    SpriteRenderer* spriteRenderer = new SpriteRenderer(ResourceManager::shaders["basic"]);
    
    while(!window.shouldClose())
    {
        window.clear();
        window.update();
    }

    ResourceManager::clear();

    return 0;
}
