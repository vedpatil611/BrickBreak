#pragma once

#include <SpriteRenderer.h>

class Level
{
public:
    Level() = default;
    Level(const char* file, unsigned int levelWidth, unsigned int levelHeight);

    void render(SpriteRenderer& spriteRenderer);
};
