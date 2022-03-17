#pragma once

#include <string>
#include <unordered_map>
#include "Shader.h"
#include "Texture.h"

class ResourceManager
{
public:
    static std::unordered_map<std::string, Shader*> shaders;
    static std::unordered_map<std::string, Texture*> textures;

    static Shader* loadShader(const char* vertFile, const char* fragFile, std::string name);
    static Texture*loadTexture(const char* path, bool alpha, std::string name);

    static void clear();
};
