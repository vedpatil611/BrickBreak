#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Window.h"

int main()
{
    Window window(600, 600);

    ResourceManager::loadShader("shader/vert.glsl", "shader/frag.glsl", "basic");
    SpriteRenderer* spriteRenderer = new SpriteRenderer(ResourceManager::shaders["basic"]);

    while(!window.shouldClose())
    {
        window.clear();
        window.update();
    }

    ResourceManager::clear();

    return 0;
}
