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
 
    int numUniforms;
    glGetProgramiv(m_Id, GL_ACTIVE_UNIFORMS, &numUniforms);

    int maxUniformLength;
    glGetProgramiv(m_Id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);

    if(numUniforms > 0 && maxUniformLength > 0)
    {
        char* uniformName = new char[maxUniformLength];
        
        for (int i = 0; i < numUniforms; ++i)
        {
            int len, size;
            GLenum dataType;

            glGetActiveUniform(m_Id, i, maxUniformLength, &len, &size, &dataType, uniformName);
            m_Cache[uniformName] = glGetUniformLocation(m_Id, uniformName);
        }

        delete[] uniformName; 
    }
 
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

void Shader::setUniform4f(const char* uniform, const glm::vec4& vec)
{
    glUniform4f(m_Cache[uniform], vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniformMat4(const char* uniform, const glm::mat4& mat)
{
    glUniformMatrix4fv(m_Cache[uniform], 1, false, &mat[0][0]);
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
