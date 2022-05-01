#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Object.h"

struct Particle
{
    glm::vec2 pos;
    glm::vec2 velocity;
    glm::vec4 color;
    float lifeTime;

    Particle(): pos(0.0f), velocity(0.0f), color(1.0f), lifeTime(0.0f) {}
};

class ParticleGenerator
{
public:
    ParticleGenerator(Shader* shader, Texture* texture, unsigned int amount);
    ~ParticleGenerator();

    void update(double delta, Object* object, unsigned int newParticles, const glm::vec2& offset = { 0.0f, 0.0f });
    void draw();
private:
    const unsigned int MAX_PARTICLES = 50;
    std::vector<Particle> m_Particles;

    struct VertexData
    {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec2 offset;
        glm::vec4 color;
    };
    
    unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;
    VertexData m_Vertices[4];

    unsigned int m_LastUsedParticle = 0;
    // unsigned int m_Amount = 0;

    Shader* m_Shader = nullptr;
    Texture* m_Tex = nullptr;

    unsigned int findUnusedParticle();
    void respawnParticle(Particle& particle, Object* object, const glm::vec2& offset);
};
