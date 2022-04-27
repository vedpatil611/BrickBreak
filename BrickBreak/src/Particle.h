#include <glm/glm.hpp>
#include <vector>

struct Particle
{
    glm::vec2 pos;
    glm::vec2 velocity, velocityVariation;
    glm::vec4 startColor, endColor;
    float lifeTime;
};

class ParticleGenerator
{
    
};
