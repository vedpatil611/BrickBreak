#include "Particle.h"
#include <glad/glad.h>

ParticleGenerator::ParticleGenerator(Shader* shader, Texture* texture, unsigned int amount)
    : m_Amount(amount), m_Shader(shader), m_Tex(texture)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    VertexData vertices[] = {
        { { 0.0f, 1.0f }, { 0.0f, 1.0f } },
        { { 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { 1.0f, 1.0f }, { 1.0f, 1.0f } },
    };

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

    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (const void *) offsetof(VertexData, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (const void *) offsetof(VertexData, uv));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Filling up particle pool
    for (unsigned int i = 0; i < MAX_PARTICLES; ++i)
        m_Particles.push_back(Particle());
}

ParticleGenerator::~ParticleGenerator()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
}

void ParticleGenerator::update(float delta, Object* object, unsigned int newParticles, const glm::vec2& offset)
{
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unUsedParticle = findUnusedParticle();
        respawnParticle(m_Particles[unUsedParticle], object, offset);
    }
    for (unsigned int i = 0; i < m_Amount; ++i)
    {
        auto& particle = m_Particles[i];
        particle.lifeTime -= delta / 1000.0f;
        if (particle.lifeTime > 0.0f)
        {
            particle.pos -= particle.velocity * delta;
            particle.color.a -= delta / 500.0f;
        }
    }
}

void ParticleGenerator::draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_Shader->bind();
    
    glActiveTexture(GL_TEXTURE0);
    m_Tex->bind();
    m_Shader->setUniform1i("uSprite", 0);

    for (Particle& particle: m_Particles)
    {
        if (particle.lifeTime > 0.0f)
        {
            m_Shader->setUniform2f("uOffset", particle.pos);
            m_Shader->setUniform4f("uColor", particle.color);

            glBindVertexArray(m_VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
            glBindVertexArray(0);
        }
    }
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
