#pragma once

#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

struct VertexData
{
    glm::vec2 pos;
    glm::vec2 uv;
};

class SpriteRenderer
{
public:
    SpriteRenderer(Shader* shader);
    ~SpriteRenderer();

    void drawSprite(Texture* texture, const glm::vec2& pos, const glm::vec2& size, 
                    float rotate, const glm::vec4& color);
private:
    Shader* m_Shader;
    unsigned int m_VAO, m_VBO, m_IBO;

    void init();
};
