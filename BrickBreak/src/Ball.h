#include "Object.h"
#include "Texture.h"

class Ball: public Object
{
public:
    float radius;
    bool stuck;
    const glm::vec2 initialVelocity;

    Ball();
    Ball(const glm::vec2& pos, float radius, glm::vec2 velocity, Texture* texture);

    glm::vec2 move(double delta, float windowWidth);
    void reset(const glm::vec2& pos);
    void reset(const glm::vec2& pos, const glm::vec2& velocity);
};
