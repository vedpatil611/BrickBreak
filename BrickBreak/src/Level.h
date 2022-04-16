#pragma once

#include "Object.h"
#include "SpriteRenderer.h"
#include <vector>

class Level
{
public:
    std::vector<Object> objects;

    Level(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    void load();

    void render(SpriteRenderer& spriteRenderer);

    inline bool isLoaded() const { return m_IsLoaded; }
private:
    const char* m_File;
    unsigned int m_Width;
    unsigned int m_Height;
    bool m_IsLoaded = false;

    void init(std::vector<std::vector<unsigned int>>& tilesData, unsigned int lvlWidth, unsigned int lvlHeight);
};
