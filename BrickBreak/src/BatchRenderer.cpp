#include "BatchRenderer.h"

#include <algorithm>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6

BatchRenderer::BatchRenderer(Shader* shader)
    :m_Shader(shader)
{
    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    vertices[0] = { { 0.0f, 1.0f }, { 0.0f, 1.0f } };
    vertices[1] = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
    vertices[2] = { { 1.0f, 0.0f }, { 1.0f, 0.0f } };
    vertices[3] = { { 1.0f, 1.0f }, { 1.0f, 1.0f } };

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, RENDERER_VERTEX_SIZE, (const void*) offsetof(VertexData, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, false, RENDERER_VERTEX_SIZE, (const void*) offsetof(VertexData, uv));
    glVertexAttribPointer(2, 4, GL_FLOAT, false, RENDERER_VERTEX_SIZE, (const void*) offsetof(VertexData, color));
    glVertexAttribPointer(3, 1, GL_FLOAT, false, RENDERER_VERTEX_SIZE, (const void*) offsetof(VertexData, tid));
    glVertexAttribPointer(4, 1, GL_FLOAT, false, RENDERER_VERTEX_SIZE, (const void*) offsetof(VertexData, index));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    unsigned short indicies[RENDERER_INDICES_SIZE] = { 0 };
    for (int i = 0, offset = 0; i < RENDERER_INDICES_SIZE; i += 6, offset += 4)
    {
        indicies[i + 0] = offset + 0;
        indicies[i + 1] = offset + 1;
        indicies[i + 2] = offset + 2;

        indicies[i + 3] = offset + 2;
        indicies[i + 4] = offset + 3;
        indicies[i + 5] = offset + 0;
    }

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BatchRenderer::begin()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_Buffer = (VertexData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void BatchRenderer::submit(Object* object)
{
    Texture* tex = object->texture;
    const glm::vec2& pos = object->pos;
    const glm::vec2& size = object->size;
    float rotate = object->rotation;
    const glm::vec4& color = object->color;
    unsigned int tid = tex->getId();

    float textureSlot = -2.0f;

    if(tid > 0)
    {
        auto it = std::find(m_TextureSlots.begin(), m_TextureSlots.end(), tid);
        if (it != m_TextureSlots.end())
        {
            textureSlot = static_cast<float>(it - m_TextureSlots.begin());
        }
        else
        {
            if (m_TextureSlots.size() >= 32)
            {
                end();
                render();
                begin();
            }
            m_TextureSlots.push_back(tid);
            textureSlot = static_cast<float>(m_TextureSlots.size() - 1);
        }
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    m_Models[m_IndexCount] = model;

    m_Buffer->pos = vertices[0].pos;
    m_Buffer->uv = vertices[0].uv;
    m_Buffer->color = color;
    m_Buffer->tid = textureSlot;
    m_Buffer->index = m_IndexCount;
    ++m_Buffer;

    m_Buffer->pos = vertices[1].pos;
    m_Buffer->uv = vertices[1].uv;
    m_Buffer->color = color;
    m_Buffer->tid = textureSlot;
    m_Buffer->index = m_IndexCount;
    ++m_Buffer;
    
    m_Buffer->pos = vertices[2].pos;
    m_Buffer->uv = vertices[2].uv;
    m_Buffer->color = color;
    m_Buffer->tid = textureSlot;
    m_Buffer->index = m_IndexCount;
    ++m_Buffer;

    m_Buffer->pos = vertices[3].pos;
    m_Buffer->uv = vertices[3].uv;
    m_Buffer->color = color;
    m_Buffer->tid = textureSlot;
    m_Buffer->index = m_IndexCount;
    ++m_Buffer;

    ++m_IndexCount;
}

void BatchRenderer::end()
{
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BatchRenderer::render()
{
    int slots[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (int i = 0; i < (int) m_TextureSlots.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
    }

    m_Shader->bind();
    m_Shader->setUniform1iv("uTextures", 10, slots);
    m_Shader->setUniformMat4("uModel", m_IndexCount, &m_Models[0][0][0]);
    
    glBindVertexArray(m_VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glDrawElements(GL_TRIANGLES, m_IndexCount * 6, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(0);

    m_TextureSlots.clear();
    m_IndexCount = 0;
}

BatchRenderer::~BatchRenderer()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteVertexArrays(1, &m_VAO);
}
