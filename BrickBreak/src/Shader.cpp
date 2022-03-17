#include "Shader.h"

#include <glad/glad.h>
#include "Utils.h"

Shader::Shader(const char* vertPath, const char* fragPath)
{
    m_Id = glCreateProgram();

    uint32_t vs = compileShader(GL_VERTEX_SHADER, vertPath);
    uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragPath);

    glAttachShader(m_Id, vs);
    glAttachShader(m_Id, fs);
    glLinkProgram(m_Id);
    glValidateProgram(m_Id);
 
    // Shader has been linked and source is no longer needed
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()
{
    glDeleteProgram(m_Id);
}

void Shader::bind() const
{
    glUseProgram(m_Id);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

uint32_t Shader::compileShader(unsigned int type, const char* shaderPath)
{
    auto id = glCreateShader(type);
    auto srcStr = Utils::readFile(shaderPath);
    const char* src = srcStr.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        printf("Failed to compile %s shader: %s\n", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", message);

        glDeleteShader(id);
        delete[] message;

        return 0;
    }

    return id;
}
