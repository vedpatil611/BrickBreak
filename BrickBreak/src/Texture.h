#pragma once

class Texture
{
public:
    Texture();
    ~Texture();

    void generate(unsigned int width, unsigned int height, unsigned char* data);
    void bind() const;

    void setInternalFormat(unsigned int internalFormat);
    void setImageFormat(unsigned int imageFormat);
private:
    unsigned int m_Id;
    unsigned int m_Width, m_Height;
    // Format of texture object
    unsigned int m_InternalFormat;
    // Format of image to be loaded
    unsigned int m_ImageFormat;

    unsigned int m_WrapS, m_WrapT;
    unsigned int m_FilterMin, m_FilterMax;
};
