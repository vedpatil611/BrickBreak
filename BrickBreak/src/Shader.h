#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader(const char* vertPath, const char* fragPath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const glm::vec4& vec);
private:
    unsigned int m_Id;
    std::unordered_map<std::string, int> m_Cache;
private:
    unsigned int compileShader(unsigned int type, const char* src);
};
