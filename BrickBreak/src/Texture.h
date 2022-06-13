#pragma once

class Texture
{
public:
    Texture();
    ~Texture();

    void generate(int width, int height, unsigned char* data);
    void bind() const;

    void setInternalFormat(int internalFormat);
    void setImageFormat(int imageFormat);

    inline const unsigned int getId() const { return m_Id; }

private:
    unsigned int m_Id;
    int m_Width, m_Height;
    // Format of texture object
    int m_InternalFormat;
    // Format of image to be loaded
    int m_ImageFormat;

    int m_WrapS, m_WrapT;
    int m_FilterMin, m_FilterMax;
};
