#include "Object.h"
#include "Texture.h"

class Ball: public Object
{
public:
    float radius;
    bool stuck;

    Ball();
    Ball(const glm::vec2& pos, float radius, glm::vec2 velocity, Texture* texture);

    glm::vec2 move(double delta, unsigned int windowWidth);
    void reset(const glm::vec2& pos, const glm::vec2& velocity);
};
