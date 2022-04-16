#include "Ball.h"

Ball::Ball()
    :Object(), radius(12.0f), stuck(true) {}

Ball::Ball(const glm::vec2& pos, float radius, glm::vec2 velocity, Texture* texture)
    :Object(pos, glm::vec2(radius * 2.0f, radius * 2.0f), texture, 0.0f, glm::vec4(1.0f), velocity), radius(radius), stuck(true) {}

glm::vec2 Ball::move(float delta, unsigned int windowWidth)
{
    if (!stuck)
    {
        pos += velocity * delta;
    }

    if (pos.x <= 0.0f)
    {
        pos.x = 0.0f;
        velocity.x *= -1;
    }
    else if (pos.x + size.x >= windowWidth)
    {
        pos.x = windowWidth - size.x;
        velocity.x *= -1;
    }
    if(pos.y <= 0.0f)
    {
        pos.y = 0.0f;
        velocity.y *= -1;
    }
    return pos;
}

void Ball::reset(const glm::vec2& pos, const glm::vec2& velocity)
{
    this->pos = pos;
    this->velocity = velocity;
    stuck = true;
}