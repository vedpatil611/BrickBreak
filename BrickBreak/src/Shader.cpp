#include "Shader.h"

#include <glad/glad.h>
#include <stdexcept>
#include "Utils.h"

Shader::Shader(const char* vertPath, const char* fragPath)
{
    m_Id = glCreateProgram();

    uint32_t vs = compileShader(GL_VERTEX_SHADER, vertPath);
    uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragPath);

    glAttachShader(m_Id, vs);
    glAttachShader(m_Id, fs);

    int result = 0;
	char eLog[1024] = { 0 };

    glLinkProgram(m_Id);
    glGetProgramiv(m_Id, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_Id, sizeof(eLog), nullptr, eLog);
		printf("Error linking program: '%s'\n", eLog);
	}

    glValidateProgram(m_Id);
    glGetProgramiv(m_Id, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_Id, sizeof(eLog), nullptr, eLog);
		printf("Error valifdating program: '%s'\n", eLog);
	}
 
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

void Shader::setUniform1i(const char* uniform, int data)
{
    if (m_Cache.find(uniform) == m_Cache.end()) findUniformLocation(uniform);
    glUniform1i(m_Cache[uniform], data);
}

void Shader::setUniform1iv(const char* uniform, int count, const int* data)
{
    if (m_Cache.find(uniform) == m_Cache.end()) findUniformLocation(uniform);
    glUniform1iv(m_Cache[uniform], count, data);
}

void Shader::setUniform2f(const char* uniform, const glm::vec2& vec)
{
    if (m_Cache.find(uniform) == m_Cache.end()) findUniformLocation(uniform);
    glUniform2f(m_Cache[uniform], vec.x, vec.y);
}

void Shader::setUniform4f(const char* uniform, const glm::vec4& vec)
{
    if (m_Cache.find(uniform) == m_Cache.end()) findUniformLocation(uniform);
    glUniform4f(m_Cache[uniform], vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniformMat4(const char* uniform, const glm::mat4& mat)
{
    if (m_Cache.find(uniform) == m_Cache.end()) findUniformLocation(uniform);
    glUniformMatrix4fv(m_Cache[uniform], 1, false, &mat[0][0]);
}

void Shader::setUniformMat4(const char* uniform, int count, const float* mat)
{
    if (m_Cache.find(uniform) == m_Cache.end()) findUniformLocation(uniform);
    glUniformMatrix4fv(m_Cache[uniform], count, false, mat);
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

int Shader::findUniformLocation(const char* uniform)
{
    int location = glGetUniformLocation(m_Id, uniform);
    if (location == -1)
    {
        char errString[64];
        sprintf(errString, "Uniform %s not found", uniform);
        throw std::runtime_error(errString);
    }
    m_Cache[uniform] = location;
    return  location;
}
