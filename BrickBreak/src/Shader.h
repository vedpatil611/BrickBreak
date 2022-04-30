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

    void setUniform1i(const char* uniform, int data);
    void setUniform1iv(const char* uniform, int count, const int* data);
    void setUniform2f(const char* uniform, const glm::vec2& vec);
    void setUniform4f(const char* uniform, const glm::vec4& vec);
    void setUniformMat4(const char* uniform, const glm::mat4& mat);
    void setUniformMat4(const char* uniform, unsigned int count, const float* mat);
private:
    unsigned int m_Id;
    std::unordered_map<std::string, int> m_Cache;
private:
    unsigned int compileShader(unsigned int type, const char* src);
    int findUniformLocation(const char* uniform);
};
