#include "SpriteRenderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(Shader* shader)
    :m_Shader(shader)
{
    const unsigned int VERTEX_BIND_POINT = 0;
    const unsigned int POS_ATTRIB_SLOT = 0;
    const unsigned int UV_ATTRIB_SLOT = 1;

    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    VertexData vertices[] = {
        { { 0.0, 1.0 }, { 0.0, 1.0 } },
        { { 1.0, 0.0 }, { 1.0, 0.0 } },
        { { 0.0, 0.0 }, { 0.0, 0.0 } },
        
        { { 0.0, 1.0 }, { 0.0, 1.0 } },
        { { 1.0, 1.0 }, { 1.0, 1.0 } },
        { { 1.0, 0.0 }, { 1.0, 0.0 } },
    };

    unsigned int vbo;
    glCreateBuffers(1, &vbo);

    glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(m_VAO, VERTEX_BIND_POINT, vbo, 0, sizeof(VertexData));

    glVertexArrayAttribFormat(m_VAO, POS_ATTRIB_SLOT, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, pos));
    glVertexArrayAttribBinding(m_VAO, POS_ATTRIB_SLOT, VERTEX_BIND_POINT);
    glEnableVertexArrayAttrib(m_VAO, POS_ATTRIB_SLOT);
 
    glVertexArrayAttribFormat(m_VAO, UV_ATTRIB_SLOT, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, uv));
    glVertexArrayAttribBinding(m_VAO, UV_ATTRIB_SLOT, VERTEX_BIND_POINT);
    glEnableVertexArrayAttrib(m_VAO, UV_ATTRIB_SLOT);

    glBindVertexArray(0);
}

void SpriteRenderer::drawSprite(Texture* texture, const glm::vec2& pos, const glm::vec2& size, 
                    float rotate, const glm::vec4& color)
{
    m_Shader->bind();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_Shader->setUniformMat4("uModel", model);
    m_Shader->setUniform4f("uColor", color);
    
    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    glBindVertexArray(this->m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
