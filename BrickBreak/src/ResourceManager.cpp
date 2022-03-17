#include "ResourceManager.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

std::unordered_map<std::string, Shader*> ResourceManager::shaders;
std::unordered_map<std::string, Texture*> ResourceManager::textures;

Shader* ResourceManager::loadShader(const char* vertFile, const char* fragFile, std::string name)
{
    Shader* shader = new Shader(vertFile, fragFile);
    shaders.insert(std::make_pair(name, shader));
    return shader;
}

Texture* ResourceManager::loadTexture(const char* path, bool alpha, std::string name)
{
    Texture* tex = new Texture();
    if (alpha)
    {
        tex->setInternalFormat(GL_RGBA);
        tex->setImageFormat(GL_RGBA);
    }

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    tex->generate(width, height, data);
    textures[name] = tex;
    
    return tex;
}

void ResourceManager::clear()
{
    for (auto [_, shader]: shaders)
        delete shader;

    for (auto [_, tex]: textures)
        delete tex;
}
