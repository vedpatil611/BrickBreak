#include "Object.h"

Object::Object()
    :pos(0.0f), size(1.0f), color(1.0f), rotation(0.0f), texture(nullptr) {}

Object::Object(const glm::vec2& pos, const glm::vec2& size, Texture* texture, float rotation, const glm::vec4& color, const glm::vec2& velocity)
    :pos(pos), size(size), color(color), rotation(rotation), velocity(velocity), texture(texture) {}

void Object::render(SpriteRenderer* spriteRenderer)
{
    spriteRenderer->drawSprite(texture, pos, size, rotation, color);
}
