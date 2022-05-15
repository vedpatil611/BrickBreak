#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Utils
{
    std::string readFile(const char* filePath);

    float randomFloat(float min, float max);

    glm::vec2 lerp(const glm::vec2& a, const glm::vec2& b, float t);
    glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t);
    glm::vec4 lerp(const glm::vec4& a, const glm::vec4& b, float t);
}
