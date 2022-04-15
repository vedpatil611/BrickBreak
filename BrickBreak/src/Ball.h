#include "Object.h"
#include "Texture.h"

class Ball
{
public:
    float radius;
    bool stuck;

    Ball();
    Ball(const glm::vec2& pos, float radius, glm::vec2 velocity, Texture* texture);
};
