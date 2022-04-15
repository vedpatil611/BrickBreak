#include "SpriteRenderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(Shader* shader)
    :m_Shader(shader)
{
    // const unsigned int VERTEX_BIND_POINT = 0;
    // const unsigned int POS_ATTRIB_SLOT = 0;
    // const unsigned int UV_ATTRIB_SLOT = 1;

    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    VertexData vertices[] = {
        { { 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 1.0f, 1.0f }, { 1.0f, 1.0f } },
    };

    /* VertexData vertices[] = {
        { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { { 1.0f, 1.0f }, { 1.0f, 1.0f } },
        
        { { 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { { 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
    }; */

    /* VertexData vertices[] = {
        { { 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { { 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
        
        { { 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { { 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { { 1.0f, 0.0f }, { 1.0f, 0.0f } },
        
        // { { 1.0f, 1.0f }, { 1.0f, 1.0f } },
        // { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
    }; */

    unsigned short indicies[] = {
        0, 1, 2,
        2, 3, 0,
    };

    glCreateBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
/*
    unsigned int ibo;
    glCreateBuffers(1, &ibo);
    glNamedBufferData(ibo, sizeof(indicies), indicies, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(m_VAO, ibo); */

    // glNamedBufferData(m_VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glVertexArrayVertexBuffer(m_VAO, VERTEX_BIND_POINT, m_VBO, 0, sizeof(VertexData));

    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (const void *) offsetof(VertexData, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (const void *) offsetof(VertexData, uv));
    glEnableVertexAttribArray(1);

    // glVertexArrayAttribFormat(m_VAO, POS_ATTRIB_SLOT, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, pos));
    // glVertexArrayAttribBinding(m_VAO, POS_ATTRIB_SLOT, VERTEX_BIND_POINT);
    // glEnableVertexArrayAttrib(m_VAO, POS_ATTRIB_SLOT);
 
    // glVertexArrayAttribFormat(m_VAO, UV_ATTRIB_SLOT, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, uv));
    // glVertexArrayAttribBinding(m_VAO, UV_ATTRIB_SLOT, VERTEX_BIND_POINT);
    // glEnableVertexArrayAttrib(m_VAO, UV_ATTRIB_SLOT);

    glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    // glDeleteBuffers(1, &m_IBO);
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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    // glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
