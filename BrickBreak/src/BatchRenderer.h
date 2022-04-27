#pragma once

#include "Object.h"
#include "Renderer.h"
#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>

#define RENDERER_MAX_SPRITES	1000

class BatchRenderer: public Renderer
{
public:
    BatchRenderer(Shader* shader);
    ~BatchRenderer() override;

    void begin();
    void submit(Object* object);
    void end();

    void render();
private:
    Shader* m_Shader;
    unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;
    unsigned int m_IndexCount = 0;

    glm::mat4 m_Models[RENDERER_MAX_SPRITES] = { glm::mat4(1.0f) };
    std::vector<int> m_TextureSlots;

    struct VertexData
    {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec4 color;
        float tid;
        float index;
    } vertices[4], *m_Buffer = nullptr;
};
