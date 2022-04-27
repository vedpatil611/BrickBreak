#include "Level.h"

#include <string>
#include <fstream>
#include <sstream>
#include "ResourceManager.h"

Level::Level(const char* file, unsigned int width, unsigned int height)
    :m_File(file), m_Width(width), m_Height(height) {}

void Level::load()
{
    objects.clear();

    std::string line;
    std::ifstream fstream(m_File);

    std::vector<std::vector<unsigned int>> tilesData;

    if (fstream) 
    {
        while (std::getline(fstream, line))
        {
            unsigned int tileCode;
            std::istringstream istream(line);
            std::vector<unsigned int> row;
            while (istream >> tileCode)
                row.push_back(tileCode);
            tilesData.push_back(row);
        }

        if (tilesData.size() > 0) init(tilesData, m_Width, m_Height);
    }

    m_IsLoaded = true;
    // for(auto& x:tilesData) { for(auto& y:x) { printf("%d ", y); } printf("\n); }
}

void Level::render(SpriteRenderer* renderer)
{
    for(auto& brick: objects)
        if (!brick.destroyed)    
            brick.render(renderer);
}

void Level::render(BatchRenderer* renderer)
{
    for(auto& brick: objects)
        if (!brick.destroyed)
            renderer->submit(&brick);
}

void Level::init(std::vector<std::vector<unsigned int>>& tilesData, unsigned int lvlWidth, unsigned int lvlHeight)
{
    unsigned int height = tilesData.size();
    unsigned int width = tilesData[0].size();

    float unitWidth = static_cast<float>(lvlWidth) / width;
    float unitHeight = static_cast<float>(lvlHeight) / height;

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            glm::vec2 pos(unitWidth * x, unitHeight * y);
            glm::vec2 size(unitWidth, unitHeight);
            if (tilesData[y][x] == 1)
            {
                Object brick(pos, size, ResourceManager::textures["solid_block"]);
                brick.isSolid = true;
                objects.push_back(brick);
            }
            else if (tilesData[y][x] > 1)
            {
                glm::vec4 color;
                switch (tilesData[y][x])
                {
                    case 2:
                        color = { 0.2f, 0.2f, 1.0f, 1.0f };
                        break;
                    case 3:
                        color = { 0.2f, 1.0f, 0.2f, 1.0f };
                        break;
                    case 4:
                        color = { 1.0f, 0.2f, 0.2f, 1.0f };
                        break;
                    case 5:
                        color = { 0.2f, 1.0f, 1.0f, 1.0f };
                        break;
                }
                objects.push_back(Object(pos, size, ResourceManager::textures["block"], 0.0f, color));
            }
        }
    }
}
