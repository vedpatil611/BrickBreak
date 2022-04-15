#include "Texture.h"

#include <glad/glad.h>

Texture::Texture()
    :m_Width(0), m_Height(0), 
    m_InternalFormat(GL_RGB), m_ImageFormat(GL_RGB), 
    m_WrapS(GL_REPEAT), m_WrapT(GL_REPEAT), 
    m_FilterMin(GL_NEAREST), m_FilterMax(GL_NEAREST)
{
    glGenTextures(1, &m_Id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_Id);
}

void Texture::generate(unsigned int width, unsigned int height, unsigned char* data)
{
    m_Width = width;
    m_Height = height;

    glBindTexture(GL_TEXTURE_2D, m_Id);
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_ImageFormat, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMax);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_Id);
}

void Texture::setInternalFormat(unsigned int internalFormat)
{
    m_InternalFormat = internalFormat;
}

void Texture::setImageFormat(unsigned int imageFormat)
{
    m_ImageFormat = imageFormat;
}
