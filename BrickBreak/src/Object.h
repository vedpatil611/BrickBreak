#pragma once

#include <glm/glm.hpp>
#include "SpriteRenderer.h"
#include "Texture.h"

class Object
{
public:
    glm::vec2 pos, size;
    glm::vec4 color;
    float rotation;

    glm::vec2 velocity;
    bool destroyed = false;
    bool isSolid = false;

    Texture* texture;

    Object();
    Object(const glm::vec2& pos, const glm::vec2& size, Texture* texture, float rotation = 0.0f, 
           const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& velocity = glm::vec2(0.0f));

    void render(SpriteRenderer* spriteRenderer);
};
