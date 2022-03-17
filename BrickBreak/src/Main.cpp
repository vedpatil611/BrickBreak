#include "ResourceManager.h"
#include "Window.h"

int main()
{
    Window window(600, 600);

    while(!window.shouldClose())
    {
        window.clear();
        window.update();
    }

    ResourceManager::clear();

    return 0;
}
