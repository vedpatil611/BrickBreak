#pragma once

#include "Object.h"
#include "SpriteRenderer.h"
#include <vector>

class Level
{
public:
    std::vector<Object> objects;
public:
    Level();
    Level(const char* file, unsigned int levelWidth, unsigned int levelHeight);

    void render(SpriteRenderer& spriteRenderer);
private:
    void init(std::vector<std::vector<unsigned int>>& tilesData, unsigned int lvlWidth, unsigned int lvlHeight);
};
