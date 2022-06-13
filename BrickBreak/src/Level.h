#pragma once

#include "Object.h"
#include "BatchRenderer.h"
#include "SpriteRenderer.h"
#include <vector>

class Level
{
public:
    std::vector<Object> objects;

    Level(const char* file, float levelWidth, float levelHeight);
    void load();
    void unload();

    void render(SpriteRenderer* renderer);
    void render(BatchRenderer* renderer);

    inline bool isLoaded() const { return m_IsLoaded; }
private:
    const char* m_File;
    float m_Width;
    float m_Height;
    bool m_IsLoaded = false;

    void init(std::vector<std::vector<unsigned int>>& tilesData, float lvlWidth, float lvlHeight);
};
