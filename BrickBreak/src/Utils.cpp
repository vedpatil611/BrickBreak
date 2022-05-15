#include "Utils.h"
#include <cmath>

#define L(x) std::lerp(a.x, b.x, t)

namespace Utils {

    std::string readFile(const char* filepath)
    {
        FILE* file = fopen(filepath, "rt");
        
        fseek(file, 0, SEEK_END);
        auto length = ftell(file);
        std::string data(length + 1, 0);
        fseek(file, 0, SEEK_SET);
        fread(data.data(), 1, length, file);
        fclose(file);

        return data;
    }

    float randomFloat(float min, float max)
    {
        float x = static_cast<float>(rand() % 100);
        return x * (max - min) / 99.0f + min;
    }

    glm::vec2 lerp(const glm::vec2& a, const glm::vec2& b, float t)
    {
        return glm::vec2(L(x), L(y));
    }

    glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t)
    {
        return glm::vec3(L(x), L(y), L(z));
    }

    glm::vec4 lerp(const glm::vec4& a, const glm::vec4& b, float t)
    {
        return glm::vec4(L(x), L(y), L(z), L(w));
    }    
}