#include "Particle.h"
#include <glad/glad.h>

ParticleGenerator::ParticleGenerator(Shader* shader, Texture* texture, unsigned int amount)
    : m_Shader(shader), m_Tex(texture)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    m_Vertices[0] = { { 0.0f, 1.0f }, { 0.0f, 1.0f } };
    m_Vertices[1] = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
    m_Vertices[2] = { { 1.0f, 0.0f }, { 1.0f, 0.0f } };
    m_Vertices[3] = { { 1.0f, 1.0f }, { 1.0f, 1.0f } };

    unsigned short indicies[MAX_PARTICLES * 6];
    for (unsigned int i = 0, offset = 0; i < MAX_PARTICLES * 6; i += 6, offset += 4)
    {
        indicies[i + 0] = offset + 0;
        indicies[i + 1] = offset + 1;
        indicies[i + 2] = offset + 2;

        indicies[i + 3] = offset + 2;
        indicies[i + 4] = offset + 3;
        indicies[i + 5] = offset + 0;
    };

    glCreateBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * 4 * MAX_PARTICLES, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(VertexData), (const void *) offsetof(VertexData, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(VertexData), (const void *) offsetof(VertexData, uv));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexData), (const void *) offsetof(VertexData, offset));
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(VertexData), (const void *) offsetof(VertexData, color));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Filling up particle pool
    m_Particles = std::vector<Particle>(MAX_PARTICLES, Particle());
}

ParticleGenerator::~ParticleGenerator()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
}

void ParticleGenerator::update(double delta, Object* object, unsigned int newParticles, const glm::vec2& offset)
{
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unUsedParticle = findUnusedParticle();
        respawnParticle(m_Particles[unUsedParticle], object, offset);
    }
    float dt = static_cast<float>(delta);

    for (auto& particle: m_Particles)
    {
        if (particle.lifeTime > 0.0f)
        {
            particle.lifeTime -= dt / 1000.0f;
            particle.pos -= particle.velocity * dt;
            particle.color.a -= dt / 500.0f;
        }
    }
}

void ParticleGenerator::draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    VertexData* data = (VertexData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (Particle& particle: m_Particles)
    {
        if (particle.lifeTime > 0.0f)
        {
            for (int i = 0; i < 4; ++i)
            {
                data->pos = m_Vertices[i].pos;
                data->uv = m_Vertices[i].uv;
                data->offset = particle.pos;
                data->color = particle.color;
                ++data;
            }
        }
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    m_Shader->bind();
    
    glActiveTexture(GL_TEXTURE0);
    m_Tex->bind();
    m_Shader->setUniform1i("uSprite", 0);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, MAX_PARTICLES * 6, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}

unsigned int ParticleGenerator::findUnusedParticle()
{
    for (unsigned int i = m_LastUsedParticle; i < MAX_PARTICLES; ++i)
    {
        if (m_Particles[i].lifeTime <= 0.0f)
        {
            m_LastUsedParticle = i;
            return i;
        }
    }

    for (unsigned int i = 0; i < m_LastUsedParticle; ++i)
    {
        if (m_Particles[i].lifeTime <= 0.0f)
        {
            m_LastUsedParticle = i;
            return i;
        }
    }

    m_LastUsedParticle = 0;
    return 0;
}


void ParticleGenerator::respawnParticle(Particle& particle, Object* object, const glm::vec2& offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.pos = object->pos + random + offset;
    particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.lifeTime = 1.0f;
    particle.velocity = object->velocity * 0.1f;
}
