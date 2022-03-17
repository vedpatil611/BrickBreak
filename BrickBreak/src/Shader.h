#pragma once

#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader(const char* vertPath, const char* fragPath);
    ~Shader();

    void bind() const;
    void unbind() const;
private:
    unsigned int m_Id;
    std::unordered_map<std::string, int> m_Cache;
private:
    unsigned int compileShader(unsigned int type, const char* src);
};
