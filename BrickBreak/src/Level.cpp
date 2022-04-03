#include "Level.h"

#include <string>
#include <fstream>
#include <sstream>
#include "ResourceManager.h"

Level::Level(const char* file, unsigned int width, unsigned int height)
{
    objects.clear();

    std::string line;
    std::ifstream fstream(file);

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

        if (tilesData.size() > 0) {}
    }
}

void Level::render(SpriteRenderer& renderer)
{
    for(auto& brick: objects)
        brick.render(renderer);
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
            glm::vec2 pos(unitWidth * x, unitWidth * y);
            glm::vec2 size(unitWidth, unitHeight);
            if (tilesData[y][x] == 1)
            {
                Object brick(pos, size, ResourceManager::textures["solid_block"]);
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